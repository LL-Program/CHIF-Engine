#pragma once
#include "CommonInclude.h"
#include "chifGraphicsDevice.h"
#include "chifEnums.h"
#include "chifMath.h"
#include "chifECS.h"
#include "chifPrimitive.h"
#include "chifVector.h"
#include "chifScene_Decl.h"
#include "chifScene_Components.h"

namespace chif
{
	class Archive;
}

namespace chif
{
	class HairParticleSystem
	{
	public:
		chif::graphics::GPUBuffer constantBuffer;
		chif::graphics::GPUBuffer generalBuffer;
		chif::scene::MeshComponent::BufferView simulation_view;
		chif::scene::MeshComponent::BufferView vb_pos[2];
		chif::scene::MeshComponent::BufferView vb_nor;
		chif::scene::MeshComponent::BufferView vb_pos_raytracing;
		chif::scene::MeshComponent::BufferView vb_uvs;
		chif::scene::MeshComponent::BufferView wetmap;
		chif::scene::MeshComponent::BufferView ib_culled;
		chif::scene::MeshComponent::BufferView indirect_view;
		chif::graphics::GPUBuffer primitiveBuffer;

		chif::graphics::GPUBuffer indexBuffer;
		chif::graphics::GPUBuffer vertexBuffer_length;

		chif::graphics::RaytracingAccelerationStructure BLAS;

		void CreateFromMesh(const chif::scene::MeshComponent& mesh);
		void CreateRenderData();
		void CreateRaytracingRenderData();

		void UpdateCPU(
			const chif::scene::TransformComponent& transform,
			const chif::scene::MeshComponent& mesh,
			float dt
		);

		struct UpdateGPUItem
		{
			const HairParticleSystem* hair = nullptr;
			uint32_t instanceIndex = 0;
			const chif::scene::MeshComponent* mesh = nullptr;
			const chif::scene::MaterialComponent* material = nullptr;
		};
		// Update a batch of hair particles by GPU
		static void UpdateGPU(
			const UpdateGPUItem* items,
			uint32_t itemCount,
			chif::graphics::CommandList cmd
		);

		mutable bool gpu_initialized = false;
		void InitializeGPUDataIfNeeded(chif::graphics::CommandList cmd);

		void Draw(
			const chif::scene::MaterialComponent& material,
			chif::enums::RENDERPASS renderPass,
			chif::graphics::CommandList cmd
		) const;

		enum FLAGS
		{
			EMPTY = 0,
			_DEPRECATED_REGENERATE_FRAME = 1 << 0,
			REBUILD_BUFFERS = 1 << 1,
			DIRTY = 1 << 2,
		};
		uint32_t _flags = EMPTY;

		chif::ecs::Entity meshID = chif::ecs::INVALID_ENTITY;

		uint32_t strandCount = 0;
		uint32_t segmentCount = 1;
		uint32_t randomSeed = 1;
		float length = 1.0f;
		float stiffness = 10.0f;
		float randomness = 0.2f;
		float viewDistance = 200;
		chif::vector<float> vertex_lengths;
		float width = 1;
		float uniformity = 1;

		struct AtlasRect
		{
			XMFLOAT4 texMulAdd = XMFLOAT4(1, 1, 0, 0);
			float size = 1;
		};
		chif::vector<AtlasRect> atlas_rects;

		// Non-serialized attributes:
		XMFLOAT4X4 world;
		chif::primitive::AABB aabb;
		chif::vector<uint32_t> indices; // it is dependent on vertex_lengths and contains triangles with non-zero lengths
		uint32_t layerMask = ~0u;
		mutable bool regenerate_frame = true;
		chif::graphics::Format position_format = chif::graphics::Format::R16G16B16A16_UNORM;
		mutable bool must_rebuild_blas = true;

		void Serialize(chif::Archive& archive, chif::ecs::EntitySerializer& seri);

		static void Initialize();

		constexpr uint32_t GetParticleCount() const { return strandCount * segmentCount; }
		uint64_t GetMemorySizeInBytes() const;

		constexpr bool IsDirty() const { return _flags & DIRTY; }
		constexpr void SetDirty(bool value = true) { if (value) { _flags |= DIRTY; } else { _flags &= ~DIRTY; } }

		void ConvertFromOLDSpriteSheet(uint32_t framesX, uint32_t framesY, uint32_t frameCount, uint32_t frameStart);
	};
}
