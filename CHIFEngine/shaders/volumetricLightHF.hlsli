#ifndef CHIF_VOLUMETRICLIGHT_HF
#define CHIF_VOLUMETRICLIGHT_HF
#include "globals.hlsli"
#include "brdf.hlsli"
#include "lightingHF.hlsli"

struct VertexToPixel {
	float4 pos			: SV_POSITION;
	float4 pos2D		: POSITION2D;
};

#endif // CHIF_VOLUMETRICLIGHT_HF
