#include "chifLoadingScreen_BindLua.h"
#include "chifScene_BindLua.h"
#include "chifTexture_BindLua.h"
#include "chifApplication_BindLua.h"
#include "chifRenderPath3D_BindLua.h"

#include <mutex>

using namespace chif::lua::scene;
using namespace chif::scene;
using namespace chif::ecs;

namespace chif::lua
{

	Luna<LoadingScreen_BindLua>::FunctionType LoadingScreen_BindLua::methods[] = {
		lunamethod(RenderPath2D_BindLua, AddSprite),
		lunamethod(RenderPath2D_BindLua, AddFont),
		lunamethod(RenderPath2D_BindLua, RemoveSprite),
		lunamethod(RenderPath2D_BindLua, RemoveFont),
		lunamethod(RenderPath2D_BindLua, ClearSprites),
		lunamethod(RenderPath2D_BindLua, ClearFonts),
		lunamethod(RenderPath2D_BindLua, GetSpriteOrder),
		lunamethod(RenderPath2D_BindLua, GetFontOrder),
		lunamethod(RenderPath2D_BindLua, AddLayer),
		lunamethod(RenderPath2D_BindLua, GetLayers),
		lunamethod(RenderPath2D_BindLua, SetLayerOrder),
		lunamethod(RenderPath2D_BindLua, SetSpriteOrder),
		lunamethod(RenderPath2D_BindLua, SetFontOrder),

		lunamethod(RenderPath_BindLua, GetLayerMask),
		lunamethod(RenderPath_BindLua, SetLayerMask),

		lunamethod(LoadingScreen_BindLua, AddLoadModelTask),
		lunamethod(LoadingScreen_BindLua, AddRenderPathActivationTask),
		lunamethod(LoadingScreen_BindLua, IsFinished),
		lunamethod(LoadingScreen_BindLua, GetProgress),
		lunamethod(LoadingScreen_BindLua, SetBackgroundTexture),
		lunamethod(LoadingScreen_BindLua, GetBackgroundTexture),
		lunamethod(LoadingScreen_BindLua, SetBackgroundMode),
		lunamethod(LoadingScreen_BindLua, GetBackgroundMode),
		{ NULL, NULL }
	};
	Luna<LoadingScreen_BindLua>::PropertyType LoadingScreen_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int LoadingScreen_BindLua::AddLoadModelTask(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading == nullptr)
		{
			chif::lua::SError(L, "AddLoadModelTask(Scene scene, string fileName, opt Matrix transform): loading screen is invalid!");
			return 0;
		}

		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			Scene_BindLua* custom_scene = Luna<Scene_BindLua>::lightcheck(L, 1);
			if (custom_scene)
			{
				// Overload 1: thread safe version
				if (argc > 1)
				{
					std::string fileName = chif::lua::SGetString(L, 2);
					XMMATRIX transform = XMMatrixIdentity();
					if (argc > 2)
					{
						Matrix_BindLua* matrix = Luna<Matrix_BindLua>::lightcheck(L, 3);
						if (matrix != nullptr)
						{
							transform = XMLoadFloat4x4(&matrix->data);
						}
						else
						{
							chif::lua::SError(L, "AddLoadModelTask(Scene scene, string fileName, opt Matrix transform) argument is not a matrix!");
						}
					}
					Entity root = CreateEntity();
					loading->addLoadingFunction([=](chif::jobsystem::JobArgs args) {
						Scene scene;
						chif::scene::LoadModel2(scene, fileName, transform, root);

						// Note: we lock the  scene for merging from multiple loading screen tasks
						//	Because we don't have fine control over thread execution in lua, and this significantly
						//	simplifies loading into scene from loadingscreen
						std::scoped_lock lck(custom_scene->scene->locker);
						custom_scene->scene->Merge(scene);
					});
					chif::lua::SSetLongLong(L, root);
					return 1;
				}
				else
				{
					chif::lua::SError(L, "AddLoadModelTask(Scene scene, string fileName, opt Matrix transform) not enough arguments!");
					return 0;
				}
			}
			else
			{
				// Overload 2: global scene version
				std::string fileName = chif::lua::SGetString(L, 1);
				XMMATRIX transform = XMMatrixIdentity();
				if (argc > 1)
				{
					Matrix_BindLua* matrix = Luna<Matrix_BindLua>::lightcheck(L, 2);
					if (matrix != nullptr)
					{
						transform = XMLoadFloat4x4(&matrix->data);
					}
					else
					{
						chif::lua::SError(L, "AddLoadModelTask(string fileName, opt Matrix transform) argument is not a matrix!");
					}
				}
				Entity root = CreateEntity();
				loading->addLoadingFunction([=](chif::jobsystem::JobArgs args) {
					Scene scene;
					chif::scene::LoadModel2(scene, fileName, transform, root);

					// Note: we lock the  scene for merging from multiple loading screen tasks
					//	Because we don't have fine control over thread execution in lua, and this significantly
					//	simplifies loading into scene from loadingscreen
					std::scoped_lock lck(GetGlobalScene()->locker);
					GetGlobalScene()->Merge(scene);
				});
				chif::lua::SSetLongLong(L, root);
				return 1;
			}
		}
		else
		{
			chif::lua::SError(L, "AddLoadModelTask(string fileName, opt Matrix transform) not enough arguments!");
		}
		return 0;
	}
	int LoadingScreen_BindLua::AddRenderPathActivationTask(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading == nullptr)
		{
			chif::lua::SError(L, "AddRenderPathActivationTask(RenderPath path, opt float fadeSeconds = 0, opt int fadeR = 0,fadeG = 0,fadeB = 0): loading screen is invalid!");
			return 0;
		}

		int argc = chif::lua::SGetArgCount(L);
		if (argc < 2)
		{
			chif::lua::SError(L, "AddRenderPathActivationTask(RenderPath path, Application app, opt float fadeSeconds = 0, opt int fadeR = 0,fadeG = 0,fadeB = 0): not enough arguments!");
			return 0;
		}
		RenderPath* path = nullptr;

		RenderPath3D_BindLua* comp3D = Luna<RenderPath3D_BindLua>::lightcheck(L, 1);
		if (comp3D == nullptr)
		{
			RenderPath2D_BindLua* comp2D = Luna<RenderPath2D_BindLua>::lightcheck(L, 1);
			if (comp2D == nullptr)
			{
				LoadingScreen_BindLua* compLoading = Luna<LoadingScreen_BindLua>::lightcheck(L, 1);
				if (compLoading == nullptr)
				{
					RenderPath_BindLua* comp = Luna<RenderPath_BindLua>::lightcheck(L, 1);
					if (comp == nullptr)
					{
						chif::lua::SError(L, "AddRenderPathActivationTask(RenderPath path, Application app, opt float fadeSeconds = 0, opt int fadeR = 0,fadeG = 0,fadeB = 0): first argument is not a RenderPath!");
						return 0;
					}
					else
					{
						path = comp->component;
					}
				}
				else
				{
					path = compLoading->component;
				}
			}
			else
			{
				path = comp2D->component;
			}
		}
		else
		{
			path = comp3D->component;
		}

		Application_BindLua* app = Luna<Application_BindLua>::lightcheck(L, 2);
		if (app == nullptr)
		{
			chif::lua::SError(L, "AddRenderPathActivationTask(RenderPath path, Application app, opt float fadeSeconds = 0, opt int fadeR = 0,fadeG = 0,fadeB = 0): second argument is not an Application!");
			return 0;
		}

		float fadeSeconds = 0;
		chif::Color fadeColor = chif::Color::Black();

		if (argc > 2)
		{
			fadeSeconds = chif::lua::SGetFloat(L, 3);
			if (argc > 3)
			{
				fadeColor.setR((uint8_t)chif::lua::SGetInt(L, 4));
				if (argc > 4)
				{
					fadeColor.setG((uint8_t)chif::lua::SGetInt(L, 5));
					if (argc > 5)
					{
						fadeColor.setB((uint8_t)chif::lua::SGetInt(L, 6));
					}
				}
			}
		}

		loading->addLoadingComponent(path, app->component, fadeSeconds, fadeColor);
		return 0;
	}
	int LoadingScreen_BindLua::IsFinished(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading != nullptr)
		{
			chif::lua::SSetBool(L, loading->isFinished());
			return 1;
		}
		chif::lua::SError(L, "IsFinished(): loading screen is invalid!");
		return 0;
	}
	int LoadingScreen_BindLua::GetProgress(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading != nullptr)
		{
			chif::lua::SSetInt(L, loading->getProgress());
			return 1;
		}
		chif::lua::SError(L, "GetProgress(): loading screen is invalid!");
		return 0;
	}
	int LoadingScreen_BindLua::SetBackgroundTexture(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading == nullptr)
		{
			chif::lua::SError(L, "SetBackgroundTexture(Texture tex): loading screen is not valid!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "SetBackgroundTexture(Texture tex): not enough arguments!");
			return 0;
		}
		Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
		if (tex == nullptr)
		{
			chif::lua::SError(L, "SetBackgroundTexture(Texture tex): argument is not a Texture!");
			return 0;
		}
		loading->backgroundTexture = tex->resource;
		return 0;
	}
	int LoadingScreen_BindLua::GetBackgroundTexture(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading == nullptr)
		{
			chif::lua::SError(L, "GetBackgroundTexture(): loading screen is not valid!");
			return 0;
		}
		Luna<Texture_BindLua>::push(L, loading->backgroundTexture);
		return 1;
	}
	int LoadingScreen_BindLua::SetBackgroundMode(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading == nullptr)
		{
			chif::lua::SError(L, "SetBackgroundMode(int mode): loading screen is not valid!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "SetBackgroundMode(int mode): not enough arguments!");
			return 0;
		}
		loading->background_mode = (LoadingScreen::BackgroundMode)chif::lua::SGetInt(L, 1);
		return 0;
	}
	int LoadingScreen_BindLua::GetBackgroundMode(lua_State* L)
	{
		LoadingScreen* loading = dynamic_cast<LoadingScreen*>(component);
		if (loading == nullptr)
		{
			chif::lua::SError(L, "GetBackgroundMode(): loading screen is not valid!");
			return 0;
		}
		chif::lua::SSetInt(L, (int)loading->background_mode);
		return 1;
	}

	void LoadingScreen_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<LoadingScreen_BindLua>::Register(chif::lua::GetLuaState());

			chif::lua::RunText(R"(
BackgroundMode = {
	Fill = 0,
	Fit = 1,
	Stretch = 2
}
)");
		}
	}

}
