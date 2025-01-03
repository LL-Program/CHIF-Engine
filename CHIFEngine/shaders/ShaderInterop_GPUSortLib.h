#ifndef CHIF_SHADERINTEROP_GPUSORTLIB_H
#define CHIF_SHADERINTEROP_GPUSORTLIB_H

#include "ShaderInterop.h"

struct SortConstants
{
	int3 job_params;
	uint counterReadOffset;
};
PUSHCONSTANT(sort, SortConstants);

#endif // CHIF_SHADERINTEROP_GPUSORTLIB_H
