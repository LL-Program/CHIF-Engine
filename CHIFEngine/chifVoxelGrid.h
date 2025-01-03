#pragma once
#include "CommonInclude.h"
#include "chifVector.h"
#include "chifMath.h"
#include "chifPrimitive.h"
#include "chifGraphicsDevice.h"
#include "chifArchive.h"
#include "chifECS.h"

namespace chif
{
	struct VoxelGrid
	{
		enum FLAGS
		{
			EMPTY = 0,
		};
		uint32_t _flags = EMPTY;

		XMUINT3 resolution = XMUINT3(0, 0, 0);
		XMUINT3 resolution_div4 = XMUINT3(0, 0, 0);
		XMFLOAT3 resolution_rcp = XMFLOAT3(0, 0, 0);
		chif::vector<uint64_t> voxels; // 1 array element stores 4 * 4 * 4 = 64 voxels

		XMFLOAT3 center = XMFLOAT3(0, 0, 0);
		XMFLOAT3 voxelSize = XMFLOAT3(0.25f, 0.25f, 0.25f);
		XMFLOAT3 voxelSize_rcp = XMFLOAT3(1.0f / 0.25f, 1.0f / 0.25f, 1.0f / 0.25f);

		XMFLOAT4 debug_color = XMFLOAT4(0.4f, 1, 0.2f, 0.1f); // color of voxels in debug
		XMFLOAT4 debug_color_extent = XMFLOAT4(1, 1, 0.2f, 1); // color of extent box in debug

		void init(uint32_t dimX, uint32_t dimY, uint32_t dimZ);
		void cleardata();
		void inject_triangle(XMVECTOR A, XMVECTOR B, XMVECTOR C, bool subtract = false);
		void inject_aabb(const chif::primitive::AABB& aabb, bool subtract = false);
		void inject_sphere(const chif::primitive::Sphere& sphere, bool subtract = false);
		void inject_capsule(const chif::primitive::Capsule& capsule, bool subtract = false);
		XMUINT3 world_to_coord(const XMFLOAT3& worldpos) const;
		XMINT3 world_to_coord_signed(const XMFLOAT3& worldpos) const;
		XMFLOAT3 coord_to_world(const XMUINT3& coord) const;
		XMFLOAT3 coord_to_world(const XMINT3& coord) const;
		bool check_voxel(const XMINT3& coord) const;
		bool check_voxel(const XMUINT3& coord) const;
		bool check_voxel(const XMFLOAT3& worldpos) const;
		bool is_coord_valid(const XMUINT3& coord) const;
		bool is_coord_valid(const XMINT3& coord) const;
		void set_voxel(const XMINT3& coord, bool value);
		void set_voxel(const XMUINT3& coord, bool value);
		void set_voxel(const XMFLOAT3& worldpos, bool value);
		size_t get_memory_size() const;
		void set_voxelsize(float size);
		void set_voxelsize(const XMFLOAT3& size);
		chif::primitive::AABB get_aabb() const;
		void from_aabb(const chif::primitive::AABB& aabb);
		bool is_visible(const XMUINT3& observer, const XMUINT3& subject) const;
		bool is_visible(const XMFLOAT3& observer, const XMFLOAT3& subject) const;
		bool is_visible(const XMFLOAT3& observer, const chif::primitive::AABB& subject) const;
		void add(const VoxelGrid& other);
		void subtract(const VoxelGrid& other);
		void flood_fill();
		void debugdraw(const XMFLOAT4X4& ViewProjection, chif::graphics::CommandList cmd) const;

		inline bool IsValid() const { return !voxels.empty(); }

		void Serialize(chif::Archive& archive, chif::ecs::EntitySerializer& seri);

		inline static XMVECTOR XM_CALLCONV world_to_uvw(XMVECTOR P, XMVECTOR center, XMVECTOR resolution_rcp, XMVECTOR voxelSize_rcp)
		{
			XMVECTOR diff = (P - center) * resolution_rcp * voxelSize_rcp;
			XMVECTOR uvw = diff * XMVectorSet(0.5f, -0.5f, 0.5f, 1) + XMVectorReplicate(0.5f);
			return uvw;
		}
		inline static XMVECTOR XM_CALLCONV uvw_to_world(XMVECTOR uvw, XMVECTOR center, XMVECTOR resolution, XMVECTOR voxelSize)
		{
			XMVECTOR P = uvw * 2 - XMVectorReplicate(1);
			P *= XMVectorSet(1, -1, 1, 1);
			P *= voxelSize;
			P *= resolution;
			P += center;
			return P;
		}

		// Create a polygon mesh from the voxel grid:
		void create_mesh(
			chif::vector<uint32_t>& indices,
			chif::vector<XMFLOAT3>& vertices,
			bool simplify
		);

//#define DEBUG_VOXEL_OCCLUSION
#ifdef DEBUG_VOXEL_OCCLUSION
		mutable chif::vector<XMUINT3> debug_subject_coords;
		mutable chif::vector<XMUINT3> debug_visible_coords;
		mutable chif::vector<XMUINT3> debug_occluded_coords;
#endif // DEBUG_VOXEL_OCCLUSION
	};

}
