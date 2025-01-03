#pragma once
#include "chifRenderPath3D.h"
#include "chifVector.h"

namespace chif
{

	class RenderPath3D_PathTracing :
		public RenderPath3D
	{
	protected:
		int sam = -1;
		int target = 1024;
		chif::graphics::Texture traceResult;
		chif::graphics::Texture traceDepth;
		chif::graphics::Texture traceStencil;

		chif::vector<uint8_t> texturedata_src;
		chif::vector<uint8_t> texturedata_dst;
		chif::vector<uint8_t> texturedata_albedo;
		chif::vector<uint8_t> texturedata_normal;
		chif::graphics::Texture denoiserAlbedo;
		chif::graphics::Texture denoiserNormal;
		chif::graphics::Texture denoiserResult;
		chif::jobsystem::context denoiserContext;

		void ResizeBuffers() override;

	public:

		void Update(float dt) override;
		void Render() const override;
		void Compose(chif::graphics::CommandList cmd) const override;

		int getCurrentSampleCount() const { return sam; }
		void setTargetSampleCount(int value) { target = value; }
		float getProgress() const { return (float)sam / (float)target; }

		float denoiserProgress = 0;
		float getDenoiserProgress() const { return denoiserProgress; }
		bool isDenoiserAvailable() const;

		void resetProgress() { sam = -1; denoiserProgress = 0; volumetriccloudResources.ResetFrame(); }

		uint8_t instanceInclusionMask_PathTrace = 0xFF;
	};

}
