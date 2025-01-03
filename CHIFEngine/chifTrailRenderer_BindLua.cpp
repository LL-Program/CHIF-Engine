#include "chifTrailRenderer_BindLua.h"
#include "chifMath_BindLua.h"
#include "chifTexture_BindLua.h"

namespace chif::lua
{
	Luna<TrailRenderer_BindLua>::FunctionType TrailRenderer_BindLua::methods[] = {
		lunamethod(TrailRenderer_BindLua, AddPoint),
		lunamethod(TrailRenderer_BindLua, Cut),
		lunamethod(TrailRenderer_BindLua, Clear),
		lunamethod(TrailRenderer_BindLua, GetPointCount),
		lunamethod(TrailRenderer_BindLua, GetPoint),
		lunamethod(TrailRenderer_BindLua, SetPoint),
		lunamethod(TrailRenderer_BindLua, SetBlendMode),
		lunamethod(TrailRenderer_BindLua, GetBlendMode),
		lunamethod(TrailRenderer_BindLua, SetSubdivision),
		lunamethod(TrailRenderer_BindLua, GetSubdivision),
		lunamethod(TrailRenderer_BindLua, SetWidth),
		lunamethod(TrailRenderer_BindLua, GetWidth),
		lunamethod(TrailRenderer_BindLua, SetColor),
		lunamethod(TrailRenderer_BindLua, GetColor),
		lunamethod(TrailRenderer_BindLua, SetTexture),
		lunamethod(TrailRenderer_BindLua, GetTexture),
		lunamethod(TrailRenderer_BindLua, SetTexture2),
		lunamethod(TrailRenderer_BindLua, GetTexture2),
		lunamethod(TrailRenderer_BindLua, SetTexMulAdd),
		lunamethod(TrailRenderer_BindLua, GetTexMulAdd),
		lunamethod(TrailRenderer_BindLua, SetTexMulAdd2),
		lunamethod(TrailRenderer_BindLua, GetTexMulAdd2),
		{ NULL, NULL }
	};
	Luna<TrailRenderer_BindLua>::PropertyType TrailRenderer_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int TrailRenderer_BindLua::AddPoint(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::AddPoint(Vector pos, opt float width = 1, opt Vector color = Vector(1,1,1,1)) not enough arguments!");
			return 0;
		}
		Vector_BindLua* pos = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (pos == nullptr)
		{
			chif::lua::SError(L, "TrailRenderer::AddPoint(Vector pos, opt float width = 1, opt Vector color = Vector(1,1,1,1)) first argument is not a Vector!");
			return 0;
		}
		TrailRenderer::TrailPoint point;
		point.position = pos->GetFloat3();

		if (argc > 1)
		{
			point.width = chif::lua::SGetFloat(L, 2);
			if (argc > 2)
			{
				Vector_BindLua* col = Luna<Vector_BindLua>::lightcheck(L, 3);
				if (col == nullptr)
				{
					chif::lua::SError(L, "TrailRenderer::AddPoint(Vector pos, opt float width = 1, opt Vector color = Vector(1,1,1,1)) third argument is not a Vector!");
				}
				else
				{
					point.color = col->data;
				}
			}
		}

		trail.points.push_back(point);
		return 0;
	}
	int TrailRenderer_BindLua::Cut(lua_State* L)
	{
		trail.Cut();
		return 0;
	}
	int TrailRenderer_BindLua::Clear(lua_State* L)
	{
		trail.points.clear();
		trail.cuts.clear();
		return 0;
	}
	int TrailRenderer_BindLua::GetPointCount(lua_State* L)
	{
		chif::lua::SSetInt(L, int(trail.points.size()));
		return 1;
	}
	int TrailRenderer_BindLua::GetPoint(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		int index = chif::lua::SGetInt(L, 1);
		if (index >= trail.points.size())
		{
			chif::lua::SError(L, "TrailRenderer::GetPoint(int index): index out of range!");
			return 0;
		}
		auto& point = trail.points[index];
		Luna<Vector_BindLua>::push(L, point.position);
		chif::lua::SSetFloat(L, point.width);
		Luna<Vector_BindLua>::push(L, point.color);
		return 3;
	}
	int TrailRenderer_BindLua::SetPoint(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 2)
		{
			chif::lua::SError(L, "TrailRenderer::SetPoint(int index, Vector pos, opt float width = 1, opt Vector color = Vector(1,1,1,1)): not enough arguments!");
			return 0;
		}
		Vector_BindLua* pos = Luna<Vector_BindLua>::lightcheck(L, 2);
		if (pos == nullptr)
		{
			chif::lua::SError(L, "TrailRenderer::SetPoint(int index, Vector pos, opt float width = 1, opt Vector color = Vector(1,1,1,1)): second argument is not a Vector!");
			return 0;
		}
		int index = chif::lua::SGetInt(L, 1);
		if (index >= trail.points.size())
		{
			chif::lua::SError(L, "TrailRenderer::SetPoint(int index, Vector pos, opt float width = 1, opt Vector color = Vector(1,1,1,1)): index out of range!");
			return 0;
		}
		auto& point = trail.points[index];
		point.position = pos->GetFloat3();
		if (argc > 2)
		{
			point.width = chif::lua::SGetFloat(L, 3);
			if (argc > 3)
			{
				Vector_BindLua* col = Luna<Vector_BindLua>::lightcheck(L, 4);
				if (col == nullptr)
				{
					chif::lua::SError(L, "TrailRenderer::SetPoint(int index, Vector pos, opt float width = 1, opt Vector color = Vector(1,1,1,1)): fourth argument is not a Vector!");
				}
				else
				{
					point.color = col->data;
				}
			}
		}
		return 0;
	}
	int TrailRenderer_BindLua::SetBlendMode(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetBlendMode(int blendmode): not enough arguments!");
			return 0;
		}
		trail.blendMode = (chif::enums::BLENDMODE)chif::lua::SGetInt(L, 1);
		return 0;
	}
	int TrailRenderer_BindLua::GetBlendMode(lua_State* L)
	{
		chif::lua::SSetInt(L, (int)trail.blendMode);
		return 1;
	}
	int TrailRenderer_BindLua::SetSubdivision(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetSubdivision(int subdiv): not enough arguments!");
			return 0;
		}
		trail.subdivision = (uint32_t)chif::lua::SGetInt(L, 1);
		return 0;
	}
	int TrailRenderer_BindLua::GetSubdivision(lua_State* L)
	{
		chif::lua::SSetInt(L, (int)trail.subdivision);
		return 1;
	}
	int TrailRenderer_BindLua::SetWidth(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetWidth(float width): not enough arguments!");
			return 0;
		}
		trail.width = chif::lua::SGetFloat(L, 1);
		return 0;
	}
	int TrailRenderer_BindLua::GetWidth(lua_State* L)
	{
		chif::lua::SSetFloat(L, trail.width);
		return 1;
	}
	int TrailRenderer_BindLua::SetColor(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetColor(Vector color): not enough arguments!");
			return 0;
		}
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec == nullptr)
		{
			chif::lua::SError(L, "TrailRenderer::SetColor(Vector color): first argument is not a Vector!");
			return 0;
		}
		trail.color = vec->data;
		return 0;
	}
	int TrailRenderer_BindLua::GetColor(lua_State* L)
	{
		Luna<Vector_BindLua>::push(L, trail.color);
		return 1;
	}
	int TrailRenderer_BindLua::SetTexture(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexture(Texture tex): not enough arguments!");
			return 0;
		}
		Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
		if (tex == nullptr)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexture(Texture tex): first argument is not a Texture!");
			return 0;
		}
		trail.texture = tex->resource.GetTexture();
		return 0;
	}
	int TrailRenderer_BindLua::GetTexture(lua_State* L)
	{
		chif::Resource res;
		res.SetTexture(trail.texture);
		Luna<Texture_BindLua>::push(L, res);
		return 1;
	}
	int TrailRenderer_BindLua::SetTexture2(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexture2(Texture tex): not enough arguments!");
			return 0;
		}
		Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
		if (tex == nullptr)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexture2(Texture tex): first argument is not a Texture!");
			return 0;
		}
		trail.texture2 = tex->resource.GetTexture();
		return 0;
	}
	int TrailRenderer_BindLua::GetTexture2(lua_State* L)
	{
		chif::Resource res;
		res.SetTexture(trail.texture2);
		Luna<Texture_BindLua>::push(L, res);
		return 1;
	}
	int TrailRenderer_BindLua::SetTexMulAdd(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexMulAdd(Vector): not enough arguments!");
			return 0;
		}
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec == nullptr)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexMulAdd(Vector): first argument is not a Vector!");
			return 0;
		}
		trail.texMulAdd = vec->data;
		return 0;
	}
	int TrailRenderer_BindLua::GetTexMulAdd(lua_State* L)
	{
		Luna<Vector_BindLua>::push(L, trail.texMulAdd);
		return 1;
	}
	int TrailRenderer_BindLua::SetTexMulAdd2(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexMulAdd2(Vector): not enough arguments!");
			return 0;
		}
		Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (vec == nullptr)
		{
			chif::lua::SError(L, "TrailRenderer::SetTexMulAdd2(Vector): first argument is not a Vector!");
			return 0;
		}
		trail.texMulAdd2 = vec->data;
		return 0;
	}
	int TrailRenderer_BindLua::GetTexMulAdd2(lua_State* L)
	{
		Luna<Vector_BindLua>::push(L, trail.texMulAdd2);
		return 1;
	}

	void TrailRenderer_BindLua::Bind()
	{
		Luna<TrailRenderer_BindLua>::Register(chif::lua::GetLuaState());
	}
}
