cmake_minimum_required(VERSION 3.12)

set(MIN_OpenImageDenoise_VERSION "2.0")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
set(INSTALL_LOCAL_CONFIGDIR "${CMAKE_BINARY_DIR}/cmake")
set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/CHIFEngine")

if (CHIF_DYNAMIC_LIBRARY)
	set(CHIF_LIBRARY_TYPE SHARED)
	message(STATUS "Building CHIFEngine as a shared library")
else()
	set(CHIF_LIBRARY_TYPE STATIC)
	message(STATUS "Building CHIFEngine as a static library")
endif()

if (WIN32)
	set(TARGET_NAME CHIFEngine_Windows)
else ()
	set(TARGET_NAME CHIFEngine_Linux)
	find_package(SDL2 REQUIRED)
	find_package(OpenImageDenoise "${MIN_OpenImageDenoise_VERSION}" QUIET)
	find_package(Threads REQUIRED)
	if(NOT ${OpenImageDenoise_FOUND})
		message("OpenImageDenoise>=${MIN_OpenImageDenoise_VERSION} not found, it will be disabled.")
	else()
		message("OpenImageDenoise ${OpenImageDenoise_VERSION} Found.")
	endif()

	if(NOT TARGET SDL2::SDL2)
		find_library(SDL2_LIBRARY_FILE_LOCATION SDL2 REQUIRED)

		add_library(SDL2::SDL2 SHARED IMPORTED)
		set_target_properties(SDL2::SDL2 PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIRS}
			INTERFACE_LINK_LIBRARIES ${SDL2_LIBRARIES}
			IMPORTED_LOCATION ${SDL2_LIBRARY_FILE_LOCATION}
		)

		unset(SDL2_LIBRARY_FILE_LOCATION)
	endif()
	if((${SDL_VERSION_MAJOR} GREATER_EQUAL 2) AND (${SDL2_VERSION_MINOR} GREATER_EQUAL 0) AND (${SDL2_VERSION_PATCH} GREATER_EQUAL 14))
		add_compile_definitions(SDL2_FEATURE_CONTROLLER_LED=1)
	endif()
endif()

add_subdirectory(LUA)
add_subdirectory(Utility)

add_compile_definitions(JPH_DEBUG_RENDERER=1)
set(PHYSICS_REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
include(${PHYSICS_REPO_ROOT}/Jolt/Jolt.cmake)

file(GLOB HEADER_FILES CONFIGURE_DEPENDS *.h)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.cpp)
list(REMOVE_ITEM SOURCE_FILES offlineshadercompiler.cpp)

add_library(${TARGET_NAME} ${CHIF_LIBRARY_TYPE}
	${SOURCE_FILES}
	${HEADER_FILES}
)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

add_library(CHIFEngine ALIAS ${TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CHIFEngine>
)

set(CHIFENGINE_STATIC_LIBRARIES
	Jolt
	LUA
	Utility
)

if (WIN32)
	target_compile_definitions(${TARGET_NAME} PUBLIC
		UNICODE _UNICODE
	)

	target_compile_options(${TARGET_NAME} PRIVATE /W3)

	set(LIBDXCOMPILER "dxcompiler.dll")
else ()
	target_compile_definitions(${TARGET_NAME} PUBLIC CHIF_UNORDERED_MAP_TYPE=2)

	target_link_libraries(${TARGET_NAME} PUBLIC
		Threads::Threads
		SDL2::SDL2
		$<$<BOOL:${OpenImageDenoise_FOUND}>:OpenImageDenoise>
	)
	set(CHIFENGINE_STATIC_LIBRARIES ${CHIFENGINE_STATIC_LIBRARIES} FAudio)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${TARGET_NAME} PRIVATE
			-Wuninitialized
			#-Wwrite-strings
			#-Winit-self
			#-Wreturn-type
			#-Wreorder
			#-Werror=delete-non-virtual-dtor
			#-Werror
			#uncomment this to stop the compilation at the first error
			# -Wfatal-errors
			)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${TARGET_NAME} PRIVATE
			-Wuninitialized
			-Wmaybe-uninitialized
			-Wwrite-strings
			-Winit-self
			-Wreturn-type
			-Wreorder
			-Werror=delete-non-virtual-dtor
			-Werror
			# -Wfatal-errors
			)
	endif()

	target_link_libraries(${TARGET_NAME} PRIVATE dl)

	set(LIBDXCOMPILER "libdxcompiler.so")
endif()

target_link_libraries(${TARGET_NAME} PUBLIC ${CHIFENGINE_STATIC_LIBRARIES})

if (PLATFORM MATCHES "SDL2")
	target_compile_definitions(${TARGET_NAME} PUBLIC SDL2=1)
endif()

add_library(dxcompiler SHARED IMPORTED)
set_property(TARGET dxcompiler PROPERTY
		IMPORTED_LOCATION ${CHIF_ROOT_DIR}/CHIFEngine/${LIBDXCOMPILER} )

add_custom_command(
		TARGET ${TARGET_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CHIF_ROOT_DIR}/CHIFEngine/${LIBDXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CHIF_ROOT_DIR}/CHIFEngine/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
)


add_executable(offlineshadercompiler
		offlineshadercompiler.cpp
)

target_link_libraries(offlineshadercompiler
		PUBLIC ${TARGET_NAME})

install(TARGETS offlineshadercompiler
		RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}/CHIFEngine")

install(DIRECTORY "${CHIF_ROOT_DIR}/Content"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/CHIFEngine")
set_target_properties("${TARGET_NAME}" PROPERTIES EXPORT_NAME "${TARGET_NAME}")


install(TARGETS "${TARGET_NAME}" ${CHIFENGINE_STATIC_LIBRARIES}
		EXPORT Engine-Targets
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/CHIFEngine"
		PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CHIFEngine")

install(FILES ${LIBDXCOMPILER}
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/CHIFEngine")

export(EXPORT Engine-Targets
		FILE "${CMAKE_BINARY_DIR}/cmake/CHIFEngineTargets.cmake"
		NAMESPACE CHIFEngine::
		)

install(EXPORT Engine-Targets
		FILE CHIFEngineTargets.cmake
		NAMESPACE CHIFEngine::
		DESTINATION ${INSTALL_CONFIGDIR})

set(_CONFIG_INSTALL_DIR_INCLUDE "${CHIF_ROOT_DIR}")
set(_CONFIG_INSTALL_DIR_LIB "${CHIF_ROOT_DIR}")
configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/CHIFEngineConfig.cmake.in
		${CMAKE_BINARY_DIR}/cmake/CHIFEngineConfig.cmake
		INSTALL_DESTINATION ${INSTALL_LOCAL_CONFIGDIR}
)
set(_CONFIG_INSTALL_DIR_INCLUDE "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
set(_CONFIG_INSTALL_DIR_LIB "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/CHIFEngineConfig.cmake.in
		${CMAKE_BINARY_DIR}/cmake/install/CHIFEngineConfig.cmake
		INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES ${CMAKE_BINARY_DIR}/cmake/install/CHIFEngineConfig.cmake
		DESTINATION ${INSTALL_CONFIGDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CHIFEngine"
		PATTERN "spirv" EXCLUDE
		PATTERN "hlsl6" EXCLUDE
		PATTERN "*.vcxitems*" EXCLUDE
		)
