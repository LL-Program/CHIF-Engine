#pragma once
#include "CommonInclude.h"
#include "chifGraphicsDevice.h"
#include "chifScene_Decl.h"

namespace chif
{
	struct GPUBVH
	{
		// Scene BVH intersection resources:
		chif::graphics::GPUBuffer bvhNodeBuffer;
		chif::graphics::GPUBuffer bvhParentBuffer;
		chif::graphics::GPUBuffer bvhFlagBuffer;
		chif::graphics::GPUBuffer primitiveCounterBuffer;
		chif::graphics::GPUBuffer primitiveIDBuffer;
		chif::graphics::GPUBuffer primitiveBuffer;
		chif::graphics::GPUBuffer primitiveMortonBuffer;
		uint32_t primitiveCapacity = 0;
		bool IsValid() const { return primitiveCounterBuffer.IsValid(); }

		void Update(const chif::scene::Scene& scene);
		void Build(const chif::scene::Scene& scene, chif::graphics::CommandList cmd) const;

		void Clear();

		static void Initialize();
	};
}
