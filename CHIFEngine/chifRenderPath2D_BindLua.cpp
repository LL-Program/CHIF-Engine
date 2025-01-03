#include "chifRenderPath2D_BindLua.h"
#include "chifRenderPath3D_BindLua.h"
#include "chifSprite_BindLua.h"
#include "chifSpriteFont_BindLua.h"

#include <string>

namespace chif::lua
{

	Luna<RenderPath2D_BindLua>::FunctionType RenderPath2D_BindLua::methods[] = {
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

		lunamethod(RenderPath2D_BindLua, CopyFrom),
		{ NULL, NULL }
	};
	Luna<RenderPath2D_BindLua>::PropertyType RenderPath2D_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int RenderPath2D_BindLua::AddSprite(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "AddSprite() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::lua::Sprite_BindLua* sprite = Luna<chif::lua::Sprite_BindLua>::lightcheck(L, 1);
			if (sprite != nullptr)
			{
				RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
				if (ccomp != nullptr)
				{
					if (argc > 1)
						ccomp->AddSprite(&sprite->sprite, chif::lua::SGetString(L, 2));
					else
						ccomp->AddSprite(&sprite->sprite);
				}
				else
				{
					chif::lua::SError(L, "AddSprite(Sprite sprite, opt string layer) not a RenderPath2D!");
				}
			}
			else
				chif::lua::SError(L, "AddSprite(Sprite sprite, opt string layer) argument is not a Sprite!");
		}
		else
		{
			chif::lua::SError(L, "AddSprite(Sprite sprite, opt string layer) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::AddFont(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "AddFont() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::lua::SpriteFont_BindLua* font = Luna<chif::lua::SpriteFont_BindLua>::lightcheck(L, 1);
			if (font != nullptr)
			{
				RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
				if (ccomp != nullptr)
				{
					if (argc > 1)
						ccomp->AddFont(&font->font, chif::lua::SGetString(L, 2));
					else
						ccomp->AddFont(&font->font);
				}
				else
				{
					chif::lua::SError(L, "AddFont(Font font, opt string layer) not a RenderPath2D!");
				}
			}
			else
				chif::lua::SError(L, "AddFont(Font font, opt string layer) argument is not a Font!");
		}
		else
		{
			chif::lua::SError(L, "AddFont(Font font, opt string layer) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::RemoveSprite(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "RemoveSprite() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::lua::Sprite_BindLua* sprite = Luna<chif::lua::Sprite_BindLua>::lightcheck(L, 1);
			if (sprite != nullptr)
			{
				RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
				if (ccomp != nullptr)
				{
					ccomp->RemoveSprite(&sprite->sprite);
				}
				else
				{
					chif::lua::SError(L, "RemoveSprite(Sprite sprite) not a RenderPath2D!");
				}
			}
			else
				chif::lua::SError(L, "RemoveSprite(Sprite sprite) argument is not a Sprite!");
		}
		else
		{
			chif::lua::SError(L, "RemoveSprite(Sprite sprite) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::RemoveFont(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "RemoveFont() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::lua::SpriteFont_BindLua* font = Luna<chif::lua::SpriteFont_BindLua>::lightcheck(L, 1);
			if (font != nullptr)
			{
				RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
				if (ccomp != nullptr)
				{
					ccomp->RemoveFont(&font->font);
				}
				else
				{
					chif::lua::SError(L, "RemoveFont(Font font) not a RenderPath2D!");
				}
			}
			else
				chif::lua::SError(L, "RemoveFont(Font font) argument is not a Font!");
		}
		else
		{
			chif::lua::SError(L, "RemoveFont(Font font) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::ClearSprites(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "ClearSprites() component is empty!");
			return 0;
		}
		RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
		if (ccomp != nullptr)
		{
			ccomp->ClearSprites();
		}
		else
		{
			chif::lua::SError(L, "ClearSprites() not a RenderPath2D!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::ClearFonts(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "ClearFonts() component is empty!");
			return 0;
		}
		RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
		if (ccomp != nullptr)
		{
			ccomp->ClearFonts();
		}
		else
		{
			chif::lua::SError(L, "ClearFonts() not a RenderPath2D!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::GetSpriteOrder(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "GetSpriteOrder() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::lua::Sprite_BindLua* sprite = Luna<chif::lua::Sprite_BindLua>::lightcheck(L, 1);
			if (sprite != nullptr)
			{
				RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
				if (ccomp != nullptr)
				{
					chif::lua::SSetInt(L, ccomp->GetSpriteOrder(&sprite->sprite));
					return 1;
				}
				else
				{
					chif::lua::SError(L, "GetSpriteOrder(Sprite sprite) not a RenderPath2D!");
				}
			}
			else
				chif::lua::SError(L, "GetSpriteOrder(Sprite sprite) argument is not a Sprite!");
		}
		else
		{
			chif::lua::SError(L, "GetSpriteOrder(Sprite sprite) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::GetFontOrder(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "GetFontOrder() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::lua::SpriteFont_BindLua* font = Luna<chif::lua::SpriteFont_BindLua>::lightcheck(L, 1);
			if (font != nullptr)
			{
				RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
				if (ccomp != nullptr)
				{
					chif::lua::SSetInt(L, ccomp->GetFontOrder(&font->font));
					return 1;
				}
				else
				{
					chif::lua::SError(L, "GetFontOrder(Font font) not a RenderPath2D!");
				}
			}
			else
				chif::lua::SError(L, "GetFontOrder(Font font) argument is not a Sprite!");
		}
		else
		{
			chif::lua::SError(L, "GetFontOrder(Font font) not enough arguments!");
		}
		return 0;
	}

	int RenderPath2D_BindLua::AddLayer(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "AddLayer() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				ccomp->AddLayer(chif::lua::SGetString(L, 1));
			}
			else
			{
				chif::lua::SError(L, "AddLayer(string name) not a RenderPath2D!");
			}
		}
		else
		{
			chif::lua::SError(L, "AddLayer(string name) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::GetLayers(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "GetLayers() component is empty!");
			return 0;
		}

		RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
		if (ccomp != nullptr)
		{
			std::string ss;
			for (auto& x : ccomp->layers)
			{
				ss += x.name + "\n";
			}
			chif::lua::SSetString(L, ss);
			return 1;
		}
		else
		{
			chif::lua::SError(L, "GetLayers() not a RenderPath2D!");
		}

		return 0;
	}
	int RenderPath2D_BindLua::SetLayerOrder(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetLayerOrder() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				ccomp->SetLayerOrder(chif::lua::SGetString(L, 1), chif::lua::SGetInt(L, 2));
			}
			else
			{
				chif::lua::SError(L, "SetLayerOrder(string name, int order) not a RenderPath2D!");
			}
		}
		else
		{
			chif::lua::SError(L, "SetLayerOrder(string name, int order) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::SetSpriteOrder(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetSpriteOrder() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				chif::lua::Sprite_BindLua* sprite = Luna<chif::lua::Sprite_BindLua>::lightcheck(L, 1);
				if (sprite != nullptr)
				{
					ccomp->SetSpriteOrder(&sprite->sprite, chif::lua::SGetInt(L, 2));
				}
				else
				{
					chif::lua::SError(L, "SetSpriteOrder(Sprite sprite, int order) argument is not a Sprite!");
				}
			}
			else
			{
				chif::lua::SError(L, "SetSpriteOrder(Sprite sprite, int order) not a RenderPath2D!");
			}
		}
		else
		{
			chif::lua::SError(L, "SetSpriteOrder(Sprite sprite, int order) not enough arguments!");
		}
		return 0;
	}
	int RenderPath2D_BindLua::SetFontOrder(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetFontOrder() component is empty!");
			return 0;
		}
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			RenderPath2D* ccomp = dynamic_cast<RenderPath2D*>(component);
			if (ccomp != nullptr)
			{
				chif::lua::SpriteFont_BindLua* font = Luna<chif::lua::SpriteFont_BindLua>::lightcheck(L, 1);
				if (font != nullptr)
				{
					ccomp->SetFontOrder(&font->font, chif::lua::SGetInt(L, 2));
				}
				else
				{
					chif::lua::SError(L, "SetFontOrder(Font font, int order) argument is not a Font!");
				}
			}
			else
			{
				chif::lua::SError(L, "SetFontOrder(Font font, int order) not a RenderPath2D!");
			}
		}
		else
		{
			chif::lua::SError(L, "SetFontOrder(Font font, int order) not enough arguments!");
		}
		return 0;
	}

	int RenderPath2D_BindLua::CopyFrom(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "CopyFrom(RenderPath other) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			RenderPath3D_BindLua* other3D = Luna<RenderPath3D_BindLua>::lightcheck(L, 1);
			if (other3D != nullptr)
			{
				*component = *other3D->component;
				return 0;
			}
			RenderPath2D_BindLua* other2D = Luna<RenderPath2D_BindLua>::lightcheck(L, 1);
			if (other2D != nullptr)
			{
				*component = *other2D->component;
				return 0;
			}
			RenderPath_BindLua* other = Luna<RenderPath_BindLua>::lightcheck(L, 1);
			if (other != nullptr)
			{
				*component = *other->component;
				return 0;
			}

			chif::lua::SError(L, "CopyFrom(RenderPath other) parameter is not a RenderPath3D!");
		}
		else
			chif::lua::SError(L, "CopyFrom(RenderPath other) not enough arguments!");
		return 0;
	}

	void RenderPath2D_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<RenderPath2D_BindLua>::Register(chif::lua::GetLuaState());
		}
	}

}
