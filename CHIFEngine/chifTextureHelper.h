#pragma once
#include "CommonInclude.h"
#include "chifGraphicsDevice.h"
#include "chifColor.h"

namespace chif::texturehelper
{
	void Initialize();

	const chif::graphics::Texture* getLogo();
	const chif::graphics::Texture* getRandom64x64();
	const chif::graphics::Texture* getColorGradeDefault();
	const chif::graphics::Texture* getNormalMapDefault();
	const chif::graphics::Texture* getBlackCubeMap();
	const chif::graphics::Texture* getUINT4();
	const chif::graphics::Texture* getBlueNoise();
	const chif::graphics::Texture* getWaterRipple();
	const chif::graphics::Texture* getCheckerBoard();

	const chif::graphics::Texture* getWhite();
	const chif::graphics::Texture* getBlack();
	const chif::graphics::Texture* getTransparent();

	bool CreateTexture(
		chif::graphics::Texture& texture,
		const void* data,
		uint32_t width,
		uint32_t height,
		chif::graphics::Format format = chif::graphics::Format::R8G8B8A8_UNORM,
		chif::graphics::Swizzle swizzle = {}
	);

	enum class GradientType
	{
		Linear,
		Circular,
		Angular,
	};
	enum class GradientFlags
	{
		None = 0,
		Inverse = 1 << 0,		// inverts resulting gradient
		Smoothstep = 1 << 1,	// applies smoothstep function to resulting gradient
		PerlinNoise = 1 << 2,	// applies perlin noise to gradient
		R16Unorm = 1 << 3,		// the texture will be created in R16_UNORM format instead of R8_UNORM
	};
	chif::graphics::Texture CreateGradientTexture(
		GradientType type,
		uint32_t width,
		uint32_t height,
		const XMFLOAT2& uv_start = XMFLOAT2(0, 0),
		const XMFLOAT2& uv_end = XMFLOAT2(1, 0),
		GradientFlags gradient_flags = GradientFlags::None,
		chif::graphics::Swizzle swizzle = { chif::graphics::ComponentSwizzle::R, chif::graphics::ComponentSwizzle::R, chif::graphics::ComponentSwizzle::R, chif::graphics::ComponentSwizzle::R },
		float perlin_scale = 1,
		uint32_t perlin_seed = 1234u,
		int perlin_octaves = 8,
		float perlin_persistence = 0.5f
	);

	// Similar to CreateGradientTexture() with GradientType::Angular type but different parameters
	chif::graphics::Texture CreateCircularProgressGradientTexture(
		uint32_t width,
		uint32_t height,
		const XMFLOAT2& direction = XMFLOAT2(0, 1),
		bool counter_clockwise = false,
		chif::graphics::Swizzle swizzle = { chif::graphics::ComponentSwizzle::R, chif::graphics::ComponentSwizzle::R, chif::graphics::ComponentSwizzle::R, chif::graphics::ComponentSwizzle::R }
	);

	// Create a lens distortion normal map (16-bit precision)
	//	width		: texture width in pixels
	//	height		: texture height in pixels
	//	uv_start	: center of lens in uv-space [0,1]
	//	radius		: radius of lens in uv_space [0,1]
	//	squish		: squish the lens (higher value is more squished down)
	//	blend		: blend out the distortion by a constant amount
	//	edge_smoothness : smoothen the edge of the circle
	chif::graphics::Texture CreateLensDistortionNormalMap(
		uint32_t width,
		uint32_t height,
		const XMFLOAT2& uv_start = XMFLOAT2(0.5f, 0.5f),
		float radius = 0.5f,
		float squish = 1,
		float blend = 1,
		float edge_smoothness = 0.04f
	);
};

template<>
struct enable_bitmask_operators<chif::texturehelper::GradientFlags> {
	static const bool enable = true;
};
