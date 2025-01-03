#include "chifPathQuery_BindLua.h"
#include "chifMath_BindLua.h"
#include "chifVoxelGrid_BindLua.h"

namespace chif::lua
{
	Luna<PathQuery_BindLua>::FunctionType PathQuery_BindLua::methods[] = {
		lunamethod(PathQuery_BindLua, Process),
		lunamethod(PathQuery_BindLua, SearchCover),
		lunamethod(PathQuery_BindLua, IsSuccessful),
		lunamethod(PathQuery_BindLua, GetNextWaypoint),
		lunamethod(PathQuery_BindLua, SetDebugDrawWaypointsEnabled),
		lunamethod(PathQuery_BindLua, SetFlying),
		lunamethod(PathQuery_BindLua, IsFlying),
		lunamethod(PathQuery_BindLua, SetAgentWidth),
		lunamethod(PathQuery_BindLua, GetAgentWidth),
		lunamethod(PathQuery_BindLua, SetAgentHeight),
		lunamethod(PathQuery_BindLua, GetAgentHeight),
		lunamethod(PathQuery_BindLua, GetWaypointCount),
		lunamethod(PathQuery_BindLua, GetWaypoint),
		lunamethod(PathQuery_BindLua, GetGoal),
		{ NULL, NULL }
	};
	Luna<PathQuery_BindLua>::PropertyType PathQuery_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int PathQuery_BindLua::Process(lua_State* L)
	{
		int args = chif::lua::SGetArgCount(L);
		if (args < 3)
		{
			chif::lua::SError(L, "PathQuery::Process(Vector start,goal, VoxelGrid voxelgrid) not enough arguments!");
			return 0;
		}
		Vector_BindLua* start = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (start == nullptr)
		{
			chif::lua::SError(L, "PathQuery::Process(Vector start,goal, VoxelGrid voxelgrid) first argument is not a Vector!");
			return 0;
		}
		Vector_BindLua* goal = Luna<Vector_BindLua>::lightcheck(L, 2);
		if (start == nullptr)
		{
			chif::lua::SError(L, "PathQuery::Process(Vector start,goal, VoxelGrid voxelgrid) second argument is not a Vector!");
			return 0;
		}
		VoxelGrid_BindLua* voxelgrid = Luna<VoxelGrid_BindLua>::lightcheck(L, 3);
		if (voxelgrid == nullptr)
		{
			chif::lua::SError(L, "PathQuery::Process(Vector start,goal, VoxelGrid voxelgrid) third argument is not a VoxelGrid!");
			return 0;
		}

		pathquery->process(start->GetFloat3(), goal->GetFloat3(), *voxelgrid->voxelgrid);

		return 0;
	}
	int PathQuery_BindLua::SearchCover(lua_State* L)
	{
		int args = chif::lua::SGetArgCount(L);
		if (args < 5)
		{
			chif::lua::SError(L, "PathQuery::SearchCover(Vector observer,subject,direction, float max_distance, VoxelGrid voxelgrid) not enough arguments!");
			return 0;
		}
		Vector_BindLua* observer = Luna<Vector_BindLua>::lightcheck(L, 1);
		if (observer == nullptr)
		{
			chif::lua::SError(L, "PathQuery::SearchCover(Vector observer,subject,direction, float max_distance, VoxelGrid voxelgrid) first argument is not a Vector!");
			return 0;
		}
		Vector_BindLua* subject = Luna<Vector_BindLua>::lightcheck(L, 2);
		if (subject == nullptr)
		{
			chif::lua::SError(L, "PathQuery::SearchCover(Vector observer,subject,direction, float max_distance, VoxelGrid voxelgrid) second argument is not a Vector!");
			return 0;
		}
		Vector_BindLua* direction = Luna<Vector_BindLua>::lightcheck(L, 3);
		if (direction == nullptr)
		{
			chif::lua::SError(L, "PathQuery::SearchCover(Vector observer,subject,direction, float max_distance, VoxelGrid voxelgrid) third argument is not a Vector!");
			return 0;
		}
		float max_distance = chif::lua::SGetFloat(L, 4);
		VoxelGrid_BindLua* voxelgrid = Luna<VoxelGrid_BindLua>::lightcheck(L, 5);
		if (voxelgrid == nullptr)
		{
			chif::lua::SError(L, "PathQuery::SearchCover(Vector observer,subject,direction, float max_distance, VoxelGrid voxelgrid) fifth argument is not a VoxelGrid!");
			return 0;
		}

		bool result = pathquery->search_cover(
			observer->GetFloat3(),
			subject->GetFloat3(),
			direction->GetFloat3(),
			max_distance,
			*voxelgrid->voxelgrid
		);

		chif::lua::SSetBool(L, result);

		return 1;
	}
	int PathQuery_BindLua::IsSuccessful(lua_State* L)
	{
		chif::lua::SSetBool(L, pathquery->is_succesful());
		return 1;
	}
	int PathQuery_BindLua::GetNextWaypoint(lua_State* L)
	{
		Luna<Vector_BindLua>::push(L, pathquery->get_next_waypoint());
		return 1;
	}
	int PathQuery_BindLua::SetDebugDrawWaypointsEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 0)
		{
			chif::lua::SError(L, "PathQuery::SetDebugDrawWaypointsEnabled(bool value) not enough arguments!");
			return 0;
		}
		pathquery->debug_waypoints = chif::lua::SGetBool(L, 1);
		return 0;
	}
	int PathQuery_BindLua::SetFlying(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 0)
		{
			chif::lua::SError(L, "PathQuery::SetFlying(bool value) not enough arguments!");
			return 0;
		}
		pathquery->flying = chif::lua::SGetBool(L, 1);
		return 0;
	}
	int PathQuery_BindLua::IsFlying(lua_State* L)
	{
		chif::lua::SSetBool(L, pathquery->flying);
		return 1;
	}
	int PathQuery_BindLua::SetAgentHeight(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 0)
		{
			chif::lua::SError(L, "PathQuery::SetAgentHeight(int value) not enough arguments!");
			return 0;
		}
		pathquery->agent_height = chif::lua::SGetInt(L, 1);
		return 0;
	}
	int PathQuery_BindLua::GetAgentHeight(lua_State* L)
	{
		chif::lua::SSetInt(L, pathquery->agent_height);
		return 1;
	}
	int PathQuery_BindLua::SetAgentWidth(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 0)
		{
			chif::lua::SError(L, "PathQuery::SetAgentWidth(int value) not enough arguments!");
			return 0;
		}
		pathquery->agent_width = chif::lua::SGetInt(L, 1);
		return 0;
	}
	int PathQuery_BindLua::GetAgentWidth(lua_State* L)
	{
		chif::lua::SSetInt(L, pathquery->agent_width);
		return 1;
	}
	int PathQuery_BindLua::GetWaypointCount(lua_State* L)
	{
		chif::lua::SSetInt(L, (int)pathquery->get_waypoint_count());
		return 1;
	}
	int PathQuery_BindLua::GetWaypoint(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "PathQuery::GetWaypoint(int index) not enough arguments!");
			return 0;
		}
		int index = chif::lua::SGetInt(L, 1);
		Luna<Vector_BindLua>::push(L, pathquery->get_waypoint(size_t(index)));
		return 1;
	}
	int PathQuery_BindLua::GetGoal(lua_State* L)
	{
		Luna<Vector_BindLua>::push(L, pathquery->get_goal());
		return 1;
	}

	void PathQuery_BindLua::Bind()
	{
		Luna<PathQuery_BindLua>::Register(chif::lua::GetLuaState());
	}
}
