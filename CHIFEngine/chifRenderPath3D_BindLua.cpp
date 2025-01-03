#include "chifRenderPath3D_BindLua.h"
#include "chifTexture_BindLua.h"

namespace chif::lua
{

	Luna<RenderPath3D_BindLua>::FunctionType RenderPath3D_BindLua::methods[] = {
		lunamethod(RenderPath2D_BindLua, AddSprite),
		lunamethod(RenderPath2D_BindLua, AddFont),
		lunamethod(RenderPath2D_BindLua, RemoveSprite),
		lunamethod(RenderPath2D_BindLua, RemoveFont),
		lunamethod(RenderPath2D_BindLua, ClearSprites),
		lunamethod(RenderPath2D_BindLua, ClearFonts),
		lunamethod(RenderPath2D_BindLua, GetSpriteOrder),
		lunamethod(RenderPath2D_BindLua, GetFontOrder),

		lunamethod(RenderPath2D_BindLua, AddLayer),
		lunamethod(RenderPath2D_BindLua, GetLayers),
		lunamethod(RenderPath2D_BindLua, SetLayerOrder),
		lunamethod(RenderPath2D_BindLua, SetSpriteOrder),
		lunamethod(RenderPath2D_BindLua, SetFontOrder),

		lunamethod(RenderPath_BindLua, GetLayerMask),
		lunamethod(RenderPath_BindLua, SetLayerMask),

		lunamethod(RenderPath3D_BindLua, SetResolutionScale),
		lunamethod(RenderPath3D_BindLua, SetAO),
		lunamethod(RenderPath3D_BindLua, SetAOPower),
		lunamethod(RenderPath3D_BindLua, SetAORange),
		lunamethod(RenderPath3D_BindLua, SetSSREnabled),
		lunamethod(RenderPath3D_BindLua, SetSSGIEnabled),
		lunamethod(RenderPath3D_BindLua, SetRaytracedDiffuseEnabled),
		lunamethod(RenderPath3D_BindLua, SetRaytracedReflectionsEnabled),
		lunamethod(RenderPath3D_BindLua, SetShadowsEnabled),
		lunamethod(RenderPath3D_BindLua, SetReflectionsEnabled),
		lunamethod(RenderPath3D_BindLua, SetFXAAEnabled),
		lunamethod(RenderPath3D_BindLua, SetBloomEnabled),
		lunamethod(RenderPath3D_BindLua, SetBloomThreshold),
		lunamethod(RenderPath3D_BindLua, SetColorGradingEnabled),
		lunamethod(RenderPath3D_BindLua, SetVolumeLightsEnabled),
		lunamethod(RenderPath3D_BindLua, SetLightShaftsEnabled),
		lunamethod(RenderPath3D_BindLua, SetLensFlareEnabled),
		lunamethod(RenderPath3D_BindLua, SetMotionBlurEnabled),
		lunamethod(RenderPath3D_BindLua, SetDitherEnabled),
		lunamethod(RenderPath3D_BindLua, SetDepthOfFieldEnabled),
		lunamethod(RenderPath3D_BindLua, SetEyeAdaptionEnabled),
		lunamethod(RenderPath3D_BindLua, SetMSAASampleCount),
		lunamethod(RenderPath3D_BindLua, SetSharpenFilterEnabled),
		lunamethod(RenderPath3D_BindLua, SetSharpenFilterAmount),
		lunamethod(RenderPath3D_BindLua, SetExposure),
		lunamethod(RenderPath3D_BindLua, SetHDRCalibration),
		lunamethod(RenderPath3D_BindLua, SetMotionBlurStrength),
		lunamethod(RenderPath3D_BindLua, SetDepthOfFieldStrength),
		lunamethod(RenderPath3D_BindLua, SetLightShaftsStrength),
		lunamethod(RenderPath3D_BindLua, SetOutlineEnabled),
		lunamethod(RenderPath3D_BindLua, SetOutlineThickness),
		lunamethod(RenderPath3D_BindLua, SetOutlineThreshold),
		lunamethod(RenderPath3D_BindLua, SetOutlineColor),
		lunamethod(RenderPath3D_BindLua, SetFSREnabled),
		lunamethod(RenderPath3D_BindLua, SetFSRSharpness),
		lunamethod(RenderPath3D_BindLua, SetFSR2Enabled),
		lunamethod(RenderPath3D_BindLua, SetFSR2Sharpness),
		lunamethod(RenderPath3D_BindLua, SetFSR2Preset),
		lunamethod(RenderPath3D_BindLua, SetTonemap),
		lunamethod(RenderPath3D_BindLua, SetVisibilityComputeShadingEnabled),

		lunamethod(RenderPath3D_BindLua, SetCropLeft),
		lunamethod(RenderPath3D_BindLua, SetCropTop),
		lunamethod(RenderPath3D_BindLua, SetCropRight),
		lunamethod(RenderPath3D_BindLua, SetCropBottom),

		lunamethod(RenderPath3D_BindLua, GetLastPostProcessRT),

		lunamethod(RenderPath3D_BindLua, SetDistortionOverlay),

		lunamethod(RenderPath2D_BindLua, CopyFrom),
		{ NULL, NULL }
	};
	Luna<RenderPath3D_BindLua>::PropertyType RenderPath3D_BindLua::properties[] = {
		{ NULL, NULL }
	};


	int RenderPath3D_BindLua::SetResolutionScale(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetResolutionScale(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			float value = chif::lua::SGetFloat(L, 1);
			((RenderPath3D*)component)->resolutionScale = value;
		}
		else
			chif::lua::SError(L, "SetResolutionScale(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetAO(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetAO(AO value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			int value = chif::lua::SGetInt(L, 1);
			RenderPath3D::AO ao = (RenderPath3D::AO)value;
			((RenderPath3D*)component)->setAO(ao);
		}
		else
			chif::lua::SError(L, "SetAO(AO value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetAOPower(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetAOPower(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			float value = chif::lua::SGetFloat(L, 1);
			((RenderPath3D*)component)->setAOPower(value);
		}
		else
			chif::lua::SError(L, "SetAOPower(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetAORange(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetAORange(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			float value = chif::lua::SGetFloat(L, 1);
			((RenderPath3D*)component)->setAORange(value);
		}
		else
			chif::lua::SError(L, "SetAORange(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetSSREnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetSSREnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setSSREnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetSSREnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetSSGIEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetSSGIEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setSSGIEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetSSGIEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetRaytracedDiffuseEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetRaytracedDiffuseEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setRaytracedDiffuseEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetRaytracedDiffuseEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetRaytracedReflectionsEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetRaytracedReflectionsEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setRaytracedReflectionsEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetRaytracedReflectionsEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetShadowsEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetShadowsEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setShadowsEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetShadowsEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetReflectionsEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetShadowsEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setReflectionsEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetShadowsEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetFXAAEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetFXAAEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setFXAAEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetFXAAEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetBloomEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetBloomEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setBloomEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetBloomEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetBloomThreshold(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetBloomThreshold(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setBloomThreshold(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetBloomThreshold(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetColorGradingEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetColorGradingEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setColorGradingEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetColorGradingEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetVolumeLightsEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetVolumeLightsEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setVolumeLightsEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetVolumeLightsEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetLightShaftsEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetLightShaftsEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setLightShaftsEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetLightShaftsEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetLensFlareEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetLensFlareEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setLensFlareEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetLensFlareEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetMotionBlurEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetMotionBlurEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setMotionBlurEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetMotionBlurEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetDitherEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetDitherEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setDitherEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetDitherEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetDepthOfFieldEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetDepthOfFieldEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setDepthOfFieldEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetDepthOfFieldEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetEyeAdaptionEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetEyeAdaptionEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setEyeAdaptionEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetEyeAdaptionEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetMSAASampleCount(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetMSAASampleCount(int value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setMSAASampleCount((uint32_t)chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "SetMSAASampleCount(int value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetSharpenFilterEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetSharpenFilterEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setSharpenFilterEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetSharpenFilterEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetSharpenFilterAmount(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetSharpenFilterAmount(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setSharpenFilterAmount(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetSharpenFilterAmount(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetExposure(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetExposure(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setExposure(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetExposure(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetHDRCalibration(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetHDRCalibration(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setHDRCalibration(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetHDRCalibration(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetMotionBlurStrength(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetMotionBlurStrength(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setMotionBlurStrength(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetMotionBlurStrength(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetDepthOfFieldStrength(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetDepthOfFieldStrength(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setDepthOfFieldStrength(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetDepthOfFieldStrength(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetLightShaftsStrength(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetLightShaftsStrength(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setLightShaftsStrength(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetLightShaftsStrength(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetOutlineEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetOutlineEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setOutlineEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetOutlineEnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetOutlineThickness(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetOutlineThickness(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setOutlineThickness(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetOutlineThickness(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetOutlineThreshold(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetOutlineThreshold(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setOutlineThreshold(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetOutlineThreshold(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetOutlineColor(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetOutlineColor(float r,g,b,a) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 3)
		{
			((RenderPath3D*)component)->setOutlineColor(chif::lua::SGetFloat4(L, 1));
		}
		else
			chif::lua::SError(L, "SetOutlineColor(float r,g,b,a) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetFSREnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetFSREnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setFSREnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetFSREnabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetFSRSharpness(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetFSRSharpness(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setFSRSharpness(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetFSRSharpness(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetFSR2Enabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetFSR2Enabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setFSR2Enabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetFSR2Enabled(bool value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetFSR2Sharpness(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetFSR2Sharpness(float value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setFSR2Sharpness(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetFSR2Sharpness(float value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetFSR2Preset(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetFSR2Preset(FSR2_Preset value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setFSR2Preset((chif::RenderPath3D::FSR2_Preset)chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "SetFSR2Preset(FSR2_Preset value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetTonemap(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetTonemap(Tonemap value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
		{
			((RenderPath3D*)component)->setTonemap((chif::renderer::Tonemap)chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "SetTonemap(Tonemap value) not enough arguments!");
		return 0;
	}
	int RenderPath3D_BindLua::SetCropLeft(lua_State* L)
	{
		((RenderPath3D*)component)->crop_left = chif::lua::SGetFloat(L, 1);
		return 0;
	}
	int RenderPath3D_BindLua::SetCropTop(lua_State* L)
	{
		((RenderPath3D*)component)->crop_top = chif::lua::SGetFloat(L, 1);
		return 0;
	}
	int RenderPath3D_BindLua::SetCropRight(lua_State* L)
	{
		((RenderPath3D*)component)->crop_right = chif::lua::SGetFloat(L, 1);
		return 0;
	}
	int RenderPath3D_BindLua::SetCropBottom(lua_State* L)
	{
		((RenderPath3D*)component)->crop_bottom = chif::lua::SGetFloat(L, 1);
		return 0;
	}
	int RenderPath3D_BindLua::SetVisibilityComputeShadingEnabled(lua_State* L)
	{
		if (component == nullptr)
		{
			chif::lua::SError(L, "SetVisibilityComputeShadingEnabled(bool value) component is null!");
			return 0;
		}
		if (chif::lua::SGetArgCount(L) > 0)
			((RenderPath3D*)component)->setVisibilityComputeShadingEnabled(chif::lua::SGetBool(L, 1));
		else
			chif::lua::SError(L, "SetVisibilityComputeShadingEnabled(bool value) not enough arguments!");
		return 0;
	}

	int RenderPath3D_BindLua::GetLastPostProcessRT(lua_State* L)
	{
		const chif::graphics::Texture* tex = ((RenderPath3D*)component)->GetLastPostprocessRT();
		if (tex == nullptr)
			return 0;
		Luna<Texture_BindLua>::push(L, *tex);
		return 1;
	}

	int RenderPath3D_BindLua::SetDistortionOverlay(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "SetDistortionOverlay(Texture texture) not enough arguments!");
			return 0;
		}
		Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
		if (tex == nullptr)
		{
			chif::lua::SError(L, "SetDistortionOverlay(Texture texture) first argument is not a Texture!");
			return 0;
		}
		if (tex->resource.IsValid())
		{
			((RenderPath3D*)component)->distortion_overlay = tex->resource.GetTexture();
		}
		else
		{
			((RenderPath3D*)component)->distortion_overlay = {};
		}
		return 0;
	}

static const std::string value_bindings = R"(
AO_DISABLED = 0
AO_SSAO = 1
AO_HBAO = 2
AO_MSAO = 3
AO_RTAO = 4

FSR2_Preset = {
	Quality = 0,
	Balanced = 1,
	Performance = 2,
	Ultra_Performance = 3,
}

Tonemap = {
	Reinhard = 0,
	ACES = 1,
}
)";

	void RenderPath3D_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<RenderPath3D_BindLua>::Register(chif::lua::GetLuaState());

			chif::lua::RunText(value_bindings);
		}
	}

}
