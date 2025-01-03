#include "chifAsync_BindLua.h"

namespace chif::lua
{
	Luna<Async_BindLua>::FunctionType Async_BindLua::methods[] = {
		lunamethod(Async_BindLua, Wait),
		lunamethod(Async_BindLua, IsCompleted),
		{ NULL, NULL }
	};
	Luna<Async_BindLua>::PropertyType Async_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int Async_BindLua::Wait(lua_State* L)
	{
		chif::jobsystem::Wait(ctx);
		return 0;
	}
	int Async_BindLua::IsCompleted(lua_State* L)
	{
		chif::lua::SSetBool(L, chif::jobsystem::IsBusy(ctx) == false);
		return 1;
	}

	void Async_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<Async_BindLua>::Register(chif::lua::GetLuaState());
		}
	}
}
