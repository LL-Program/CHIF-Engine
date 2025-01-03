#include "chifBacklog_BindLua.h"
#include "chifBacklog.h"
#include "chifLua.h"
#include "chifMath_BindLua.h"

#include <string>

namespace chif::lua::backlog
{
	int backlog_clear(lua_State* L)
	{
		chif::backlog::clear();
		return 0;
	}
	int backlog_post(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);

		std::string ss;

		for (int i = 1; i <= argc; i++)
		{
			if (chif::lua::SIsString(L, i))
			{
				ss += chif::lua::SGetString(L, i);
			}
			else if (chif::lua::SIsNumber(L, i))
			{
				double arg = chif::lua::SGetDouble(L, i);
				ss += std::to_string(arg);
			}
			else if (chif::lua::SIsBool(L, i)) {
				bool arg = chif::lua::SGetBool(L, i);
				ss += arg ? "true" : "false";
			}
			else if (chif::lua::SIsNil(L, i)) {
				ss += "nil";
			}
			else
			{
				Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, i);
				if (vec != nullptr)
				{
					ss += "Vector(" + std::to_string(vec->data.x) + ", " + std::to_string(vec->data.y) + ", " + std::to_string(vec->data.z) + ", " + std::to_string(vec->data.w) + ")";
				}
				else
				{
					Matrix_BindLua* mat = Luna<Matrix_BindLua>::lightcheck(L, i);
					if (mat != nullptr)
					{
						ss += "Matrix(\n";
						ss += "\t" + std::to_string(mat->data._11) + ", " + std::to_string(mat->data._12) + ", " + std::to_string(mat->data._13) + ", " + std::to_string(mat->data._14) + "\n";
						ss += "\t" + std::to_string(mat->data._21) + ", " + std::to_string(mat->data._22) + ", " + std::to_string(mat->data._23) + ", " + std::to_string(mat->data._24) + "\n";
						ss += "\t" + std::to_string(mat->data._31) + ", " + std::to_string(mat->data._32) + ", " + std::to_string(mat->data._33) + ", " + std::to_string(mat->data._34) + "\n";
						ss += "\t" + std::to_string(mat->data._41) + ", " + std::to_string(mat->data._42) + ", " + std::to_string(mat->data._43) + ", " + std::to_string(mat->data._44) + "\n";
						ss += ")";
					}
				}
			}
		}

		if (!ss.empty())
		{
			chif::backlog::post(ss);
		}

		return 0;
	}
	int backlog_fontsize(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);

		if (argc > 0)
		{
			chif::backlog::setFontSize(chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "backlog_fontsize(int val) not enough arguments!");

		return 0;
	}
	int backlog_isactive(lua_State* L)
	{
		chif::lua::SSetBool(L, chif::backlog::isActive());
		return 1;
	}
	int backlog_fontrowspacing(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::backlog::setFontRowspacing(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "backlog_fontrowspacing(int val) not enough arguments!");
		return 0;
	}
	int backlog_setlevel(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::backlog::SetLogLevel((chif::backlog::LogLevel)chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "backlog_setlevel(int val) not enough arguments!");
		return 0;
	}
	int backlog_lock(lua_State* L)
	{
		chif::backlog::Lock();
		return 0;
	}
	int backlog_unlock(lua_State* L)
	{
		chif::backlog::Unlock();
		return 0;
	}
	int backlog_blocklua(lua_State* L)
	{
		chif::backlog::BlockLuaExecution();
		return 0;
	}
	int backlog_unblocklua(lua_State* L)
	{
		chif::backlog::UnblockLuaExecution();
		return 0;
	}

	void Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			chif::lua::RegisterFunc("backlog_clear", backlog_clear);
			chif::lua::RegisterFunc("backlog_post", backlog_post);
			chif::lua::RegisterFunc("backlog_fontsize", backlog_fontsize);
			chif::lua::RegisterFunc("backlog_isactive", backlog_isactive);
			chif::lua::RegisterFunc("backlog_fontrowspacing", backlog_fontrowspacing);
			chif::lua::RegisterFunc("backlog_setlevel", backlog_setlevel);
			chif::lua::RegisterFunc("backlog_lock", backlog_lock);
			chif::lua::RegisterFunc("backlog_unlock", backlog_unlock);
			chif::lua::RegisterFunc("backlog_blocklua", backlog_blocklua);
			chif::lua::RegisterFunc("backlog_unblocklua", backlog_unblocklua);
		}
	}
}
