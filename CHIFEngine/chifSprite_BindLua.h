#pragma once
#include "chifLua.h"
#include "chifLuna.h"
#include "chifSprite.h"

namespace chif::lua
{
	class Sprite_BindLua
	{
	public:
		chif::Sprite sprite;

		inline static constexpr char className[] = "Sprite";
		static Luna<Sprite_BindLua>::FunctionType methods[];
		static Luna<Sprite_BindLua>::PropertyType properties[];

		Sprite_BindLua(const chif::Sprite& sprite);
		Sprite_BindLua(lua_State* L);

		int SetParams(lua_State* L);
		int GetParams(lua_State* L);
		int SetAnim(lua_State* L);
		int GetAnim(lua_State* L);
		int SetTexture(lua_State* L);
		int GetTexture(lua_State* L);
		int SetMaskTexture(lua_State* L);
		int GetMaskTexture(lua_State* L);
		int SetBackgroundTexture(lua_State* L);
		int GetBackgroundTexture(lua_State* L);
		int SetHidden(lua_State* L);
		int IsHidden(lua_State* L);

		static void Bind();
	};
}
