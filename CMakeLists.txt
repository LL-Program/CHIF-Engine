cmake_minimum_required(VERSION 3.12)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "In-source builds are not supported!\n"
        "Run `git clean -d -f` to clean up the files CMake has created (stash "
        "your changes first, if you have made any), then run `cmake -B build "
        "<other_options>` followed by `cmake --build build --parallel`"
    )
endif()

set(CHIF_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

option(CHIF_DYNAMIC_LIBRARY "Build CHIFEngine as a dynamic library" OFF)
option(USE_LIBCXX "Link CHIFEngine to llvm libc++ library - only available with the Clang compiler" OFF)

option(CHIF_EDITOR "Build CHIFEngine editor" ON)
option(CHIF_TESTS "Build CHIFEngine tests" ON)
option(CHIF_IMGUI_EXAMPLE "Build CHIFEngine imgui example" ON)

include(CMakeDependentOption)

if(UNIX)
    option(CHIF_LINUX_TEMPLATE "Build CHIFEngine Linux template" ON)
elseif(WIN32)
    option(CHIF_WINDOWS_TEMPLATE "Build CHIFEngine Windows template" ON)
endif()

# Configure CMake global variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use solution folders to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(CHIFEngine)

if (WIN32)
    set(PLATFORM "Windows")
    add_compile_definitions(WIN32=1)
    # add_compile_definitions(_WIN32=1) this is a given from the compiler
    set(DXC_TARGET "${CMAKE_CURRENT_SOURCE_DIR}/CHIFEngine/dxc.exe")
elseif(UNIX)
    set(PLATFORM "SDL2")
    add_compile_definitions(SDL2=1)
    set(DXC_TARGET "dxc")
endif()


if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec -fms-extensions")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} $<$<NOT:$<CHIF_DYNAMIC_LIBRARY>>:--for-linker=-no-pie>" )
	if (USE_LIBCXX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
	endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
else()
endif()


add_subdirectory(CHIFEngine)
add_subdirectory(Content)

if (CHIF_EDITOR)
    add_subdirectory(Editor)
endif()

if (CHIF_TESTS)
    add_subdirectory(Samples/Tests)
endif()

if (CHIF_IMGUI_EXAMPLE)
    add_subdirectory(Samples/Example_ImGui)
    add_subdirectory(Samples/Example_ImGui_Docking)
endif()

if (CHIF_LINUX_TEMPLATE)
    add_subdirectory(Samples/Template_Linux)
endif()

if (CHIF_WINDOWS_TEMPLATE)
    add_subdirectory(Samples/Template_Windows)
endif()
