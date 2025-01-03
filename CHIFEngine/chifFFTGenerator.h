#pragma once
#include "CommonInclude.h"
#include "chifGraphicsDevice.h"

namespace chif::fftgenerator
{
	void fft_512x512_c2c(
		const chif::graphics::GPUResource& pUAV_Dst,
		const chif::graphics::GPUResource& pSRV_Dst,
		const chif::graphics::GPUResource& pSRV_Src,
		chif::graphics::CommandList cmd);

	void LoadShaders();
}
