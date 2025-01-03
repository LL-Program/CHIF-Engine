#ifndef CHIF_SHADERINTEROP_RAYTRACING_H
#define CHIF_SHADERINTEROP_RAYTRACING_H
#include "ShaderInterop.h"


static const uint RAYTRACING_LAUNCH_BLOCKSIZE = 8;


CBUFFER(RaytracingCB, CBSLOT_RENDERER_TRACED)
{
	float2 xTracePixelOffset;
	uint xTraceSampleIndex;
	float xTraceAccumulationFactor;
	uint2 xTraceResolution;
	float2 xTraceResolution_rcp;
	uint4 xTraceUserData;
};


#endif // CHIF_SHADERINTEROP_RAYTRACING_H
