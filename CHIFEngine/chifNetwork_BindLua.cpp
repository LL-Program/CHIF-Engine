#include "chifNetwork_BindLua.h"
#include "chifNetwork.h"
#include "chifHelper.h"

namespace chif::lua
{

	Luna<Network_BindLua>::FunctionType Network_BindLua::methods[] = {
		{ NULL, NULL }
	};
	Luna<Network_BindLua>::PropertyType Network_BindLua::properties[] = {
		{ NULL, NULL }
	};

	void Network_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<Network_BindLua>::Register(chif::lua::GetLuaState());
			Luna<Network_BindLua>::push_global(chif::lua::GetLuaState(), "network");
		}
	}

}
