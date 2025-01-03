#pragma once
#include "CommonInclude.h"
#include "chifVector.h"
#include "chifGraphicsDevice.h"
#include "chifEnums.h"
#include "chifScene_Decl.h"

namespace chif
{
	struct TrailRenderer
	{
		struct TrailPoint
		{
			XMFLOAT3 position = XMFLOAT3(0, 0, 0);
			float width = 1;
			XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);
		};
		chif::vector<TrailPoint> points;
		chif::vector<uint32_t> cuts;
		XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);
		chif::enums::BLENDMODE blendMode = chif::enums::BLENDMODE_ALPHA;
		uint32_t subdivision = 100;
		float width = 1;
		chif::graphics::Texture texture;
		chif::graphics::Texture texture2;
		XMFLOAT4 texMulAdd = XMFLOAT4(1, 1, 0, 0);
		XMFLOAT4 texMulAdd2 = XMFLOAT4(1, 1, 0, 0);

		void Cut();

		void Draw(const chif::scene::CameraComponent& camera, chif::graphics::CommandList cmd) const;

		static void Initialize();
	};
}
