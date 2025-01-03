#pragma once
#include "CommonInclude.h"
#include "chifUnorderedMap.h"
#include "chifVector.h"
#include "chifVoxelGrid.h"
#include "chifGraphicsDevice.h"
#include "chifPrimitive.h"

#include <queue>

namespace chif
{
	struct PathQuery
	{
		struct Node
		{
			uint16_t x = 0;
			uint16_t y = 0;
			uint16_t z = 0;
			uint16_t cost = 0;
			constexpr XMUINT3 coord() const
			{
				return XMUINT3(x, y, z);
			}
			static constexpr Node create(XMUINT3 coord)
			{
				Node node = {};
				node.x = coord.x;
				node.y = coord.y;
				node.z = coord.z;
				return node;
			}
			static constexpr Node invalid()
			{
				return {};
			}
			constexpr bool operator>(const Node& other) const { return cost > other.cost; } // for priority_queue
			constexpr bool operator<(const Node& other) const { return cost < other.cost; } // for priority_queue
			constexpr operator uint64_t() const { return uint64_t(uint64_t(x) | (uint64_t(y) << 16ull) | (uint64_t(z) << 32ull)); } // for unordered_map
		};

		std::priority_queue<Node, chif::vector<Node>, std::greater<Node>> frontier;
		chif::unordered_map<uint64_t, Node> came_from;
		chif::unordered_map<uint64_t, uint16_t> cost_so_far;
		chif::vector<XMFLOAT3> result_path_goal_to_start;
		chif::vector<XMFLOAT3> result_path_goal_to_start_simplified;
		XMFLOAT3 process_startpos = XMFLOAT3(0, 0, 0);
		bool flying = false; // if set to true, it will switch to navigating on empty voxels
		int agent_height = 1; // keep away from vertical obstacles by this many voxels
		int agent_width = 0; // keep away from horizontal obstacles by this many voxels

		// Find the path between startpos and goalpos in the voxel grid:
		void process(
			const XMFLOAT3& startpos,
			const XMFLOAT3& goalpos,
			const chif::VoxelGrid& voxelgrid
		);

		bool is_succesful() const;

		// Search for a cover location that can hide the subject from observer.
		//	observer:		location to hide from
		//	subject:		location of who wants to hide
		//	direction:		subject's search direction
		//	max_distance:	maximum search distance
		//	voxelgrid:		voxel grid to perform the search in
		//	returns whether a suitable cover was found or not
		bool search_cover(
			const XMFLOAT3& observer,
			const XMFLOAT3& subject,
			const XMFLOAT3& direction,
			float max_distance,
			const chif::VoxelGrid& voxelgrid
		);

		// Gets the next upcoming waypoint between start and goal that was used in process():
		XMFLOAT3 get_next_waypoint() const;

		// Gets the number of resulting waypoints
		size_t get_waypoint_count() const;

		// Gets the result waypoint at specified index (direction: start -> goal)
		XMFLOAT3 get_waypoint(size_t index) const;

		XMFLOAT3 get_goal() const;

		bool is_voxel_valid(const VoxelGrid& voxelgrid, XMUINT3 coord) const;

		bool debug_voxels = true;
		mutable float debugtimer = 0;
		XMFLOAT3 debugvoxelsize = XMFLOAT3(0, 0, 0);
		XMFLOAT3 debugstartnode = XMFLOAT3(0, 0, 0);
		XMFLOAT3 debuggoalnode = XMFLOAT3(0, 0, 0);
		bool debug_waypoints = false; // if true, waypoint voxels will be drawn. Blue = waypoint, Pink = simplified waypoint
		void debugdraw(const XMFLOAT4X4& ViewProjection, chif::graphics::CommandList cmd) const;
	};
}
