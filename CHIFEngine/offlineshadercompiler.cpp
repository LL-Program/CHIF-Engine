#include "CHIFEngine.h"

#include <iostream>
#include <iomanip>
#include <mutex>
#include <string>
#include <cstdlib>

std::mutex locker;
struct ShaderEntry
{
	std::string name;
	chif::graphics::ShaderStage stage = chif::graphics::ShaderStage::Count;
	chif::graphics::ShaderModel minshadermodel = chif::graphics::ShaderModel::SM_5_0;
	struct Permutation
	{
		chif::vector<std::string> defines;
	};
	chif::vector<Permutation> permutations;
};
chif::vector<ShaderEntry> shaders = {
	{"hairparticle_simulateCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_simulateCS", chif::graphics::ShaderStage::CS},
	{"generateMIPChainCubeCS_float4", chif::graphics::ShaderStage::CS},
	{"generateMIPChainCubeCS_unorm4", chif::graphics::ShaderStage::CS},
	{"generateMIPChainCubeArrayCS_float4", chif::graphics::ShaderStage::CS},
	{"generateMIPChainCubeArrayCS_unorm4", chif::graphics::ShaderStage::CS},
	{"generateMIPChain3DCS_float4", chif::graphics::ShaderStage::CS},
	{"generateMIPChain3DCS_unorm4", chif::graphics::ShaderStage::CS},
	{"generateMIPChain2DCS_float4", chif::graphics::ShaderStage::CS},
	{"generateMIPChain2DCS_unorm4", chif::graphics::ShaderStage::CS},
	{"blockcompressCS_BC1", chif::graphics::ShaderStage::CS},
	{"blockcompressCS_BC3", chif::graphics::ShaderStage::CS},
	{"blockcompressCS_BC4", chif::graphics::ShaderStage::CS},
	{"blockcompressCS_BC5", chif::graphics::ShaderStage::CS},
	{"blockcompressCS_BC6H", chif::graphics::ShaderStage::CS},
	{"blockcompressCS_BC6H_cubemap", chif::graphics::ShaderStage::CS},
	{"blur_gaussian_float4CS", chif::graphics::ShaderStage::CS},
	{"bloomseparateCS", chif::graphics::ShaderStage::CS},
	{"depthoffield_mainCS", chif::graphics::ShaderStage::CS},
	{"depthoffield_neighborhoodMaxCOCCS", chif::graphics::ShaderStage::CS},
	{"depthoffield_prepassCS", chif::graphics::ShaderStage::CS},
	{"depthoffield_upsampleCS", chif::graphics::ShaderStage::CS},
	{"depthoffield_tileMaxCOC_verticalCS", chif::graphics::ShaderStage::CS},
	{"depthoffield_tileMaxCOC_horizontalCS", chif::graphics::ShaderStage::CS},
	{"vxgi_offsetprevCS", chif::graphics::ShaderStage::CS},
	{"vxgi_temporalCS", chif::graphics::ShaderStage::CS},
	{"vxgi_sdf_jumpfloodCS", chif::graphics::ShaderStage::CS},
	{"vxgi_resolve_diffuseCS", chif::graphics::ShaderStage::CS},
	{"vxgi_resolve_specularCS", chif::graphics::ShaderStage::CS},
	{"upsample_bilateral_float1CS", chif::graphics::ShaderStage::CS},
	{"upsample_bilateral_float4CS", chif::graphics::ShaderStage::CS},
	{"upsample_bilateral_unorm1CS", chif::graphics::ShaderStage::CS},
	{"upsample_bilateral_unorm4CS", chif::graphics::ShaderStage::CS},
	{"temporalaaCS", chif::graphics::ShaderStage::CS},
	{"tonemapCS", chif::graphics::ShaderStage::CS},
	{"underwaterCS", chif::graphics::ShaderStage::CS},
	{"fsr_upscalingCS", chif::graphics::ShaderStage::CS},
	{"fsr_sharpenCS", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_autogen_reactive_pass", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_compute_luminance_pyramid_pass", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_prepare_input_color_pass", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_reconstruct_previous_depth_pass", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_depth_clip_pass", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_lock_pass", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_accumulate_pass", chif::graphics::ShaderStage::CS},
	{"ffx-fsr2/ffx_fsr2_rcas_pass", chif::graphics::ShaderStage::CS},
	{"ssaoCS", chif::graphics::ShaderStage::CS},
	{"ssgi_deinterleaveCS", chif::graphics::ShaderStage::CS},
	{"ssgiCS", chif::graphics::ShaderStage::CS},
	{"ssgi_upsampleCS", chif::graphics::ShaderStage::CS},
	{"rtdiffuseCS", chif::graphics::ShaderStage::CS, chif::graphics::ShaderModel::SM_6_5},
	{"rtdiffuse_spatialCS", chif::graphics::ShaderStage::CS},
	{"rtdiffuse_temporalCS", chif::graphics::ShaderStage::CS},
	{"rtdiffuse_upsampleCS", chif::graphics::ShaderStage::CS},
	{"rtreflectionCS", chif::graphics::ShaderStage::CS, chif::graphics::ShaderModel::SM_6_5},
	{"ssr_tileMaxRoughness_horizontalCS", chif::graphics::ShaderStage::CS},
	{"ssr_tileMaxRoughness_verticalCS", chif::graphics::ShaderStage::CS},
	{"ssr_depthHierarchyCS", chif::graphics::ShaderStage::CS},
	{"ssr_resolveCS", chif::graphics::ShaderStage::CS},
	{"ssr_temporalCS", chif::graphics::ShaderStage::CS},
	{"ssr_upsampleCS", chif::graphics::ShaderStage::CS},
	{"ssr_raytraceCS", chif::graphics::ShaderStage::CS},
	{"ssr_raytraceCS_cheap", chif::graphics::ShaderStage::CS},
	{"ssr_raytraceCS_earlyexit", chif::graphics::ShaderStage::CS},
	{"sharpenCS", chif::graphics::ShaderStage::CS},
	{"skinningCS", chif::graphics::ShaderStage::CS},
	{"resolveMSAADepthStencilCS", chif::graphics::ShaderStage::CS},
	{"raytraceCS", chif::graphics::ShaderStage::CS},
	{"raytraceCS_rtapi", chif::graphics::ShaderStage::CS, chif::graphics::ShaderModel::SM_6_5},
	{"paint_textureCS", chif::graphics::ShaderStage::CS},
	{"oceanUpdateDisplacementMapCS", chif::graphics::ShaderStage::CS},
	{"oceanUpdateGradientFoldingCS", chif::graphics::ShaderStage::CS},
	{"oceanSimulatorCS", chif::graphics::ShaderStage::CS},
	{"msao_interleaveCS", chif::graphics::ShaderStage::CS},
	{"msao_preparedepthbuffers1CS", chif::graphics::ShaderStage::CS},
	{"msao_preparedepthbuffers2CS", chif::graphics::ShaderStage::CS},
	{"msao_blurupsampleCS", chif::graphics::ShaderStage::CS},
	{"msao_blurupsampleCS_blendout", chif::graphics::ShaderStage::CS},
	{"msao_blurupsampleCS_premin", chif::graphics::ShaderStage::CS},
	{"msao_blurupsampleCS_premin_blendout", chif::graphics::ShaderStage::CS},
	{"msaoCS", chif::graphics::ShaderStage::CS},
	{"motionblur_neighborhoodMaxVelocityCS", chif::graphics::ShaderStage::CS},
	{"motionblur_tileMaxVelocity_horizontalCS", chif::graphics::ShaderStage::CS},
	{"motionblur_tileMaxVelocity_verticalCS", chif::graphics::ShaderStage::CS},
	{"luminancePass2CS", chif::graphics::ShaderStage::CS},
	{"motionblurCS", chif::graphics::ShaderStage::CS},
	{"motionblurCS_cheap", chif::graphics::ShaderStage::CS},
	{"motionblurCS_earlyexit", chif::graphics::ShaderStage::CS},
	{"luminancePass1CS", chif::graphics::ShaderStage::CS},
	{"lightShaftsCS", chif::graphics::ShaderStage::CS},
	{"lightCullingCS_ADVANCED_DEBUG", chif::graphics::ShaderStage::CS},
	{"lightCullingCS_DEBUG", chif::graphics::ShaderStage::CS},
	{"lightCullingCS", chif::graphics::ShaderStage::CS},
	{"lightCullingCS_ADVANCED", chif::graphics::ShaderStage::CS},
	{"hbaoCS", chif::graphics::ShaderStage::CS},
	{"gpusortlib_sortInnerCS", chif::graphics::ShaderStage::CS},
	{"gpusortlib_sortStepCS", chif::graphics::ShaderStage::CS},
	{"gpusortlib_kickoffSortCS", chif::graphics::ShaderStage::CS},
	{"gpusortlib_sortCS", chif::graphics::ShaderStage::CS},
	{"fxaaCS", chif::graphics::ShaderStage::CS},
	{"filterEnvMapCS", chif::graphics::ShaderStage::CS},
	{"fft_512x512_c2c_CS", chif::graphics::ShaderStage::CS},
	{"fft_512x512_c2c_v2_CS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_sphpartitionCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_sphcellallocationCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_sphbinningCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_simulateCS_SORTING", chif::graphics::ShaderStage::CS},
	{"emittedparticle_simulateCS_SORTING_DEPTHCOLLISIONS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_sphdensityCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_sphforceCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_kickoffUpdateCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_simulateCS_DEPTHCOLLISIONS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_emitCS", chif::graphics::ShaderStage::CS},
	{"emittedparticle_emitCS_FROMMESH", chif::graphics::ShaderStage::CS},
	{"emittedparticle_emitCS_volume", chif::graphics::ShaderStage::CS},
	{"emittedparticle_finishUpdateCS", chif::graphics::ShaderStage::CS},
	{"downsample4xCS", chif::graphics::ShaderStage::CS},
	{"lineardepthCS", chif::graphics::ShaderStage::CS},
	{"depthoffield_prepassCS_earlyexit", chif::graphics::ShaderStage::CS},
	{"depthoffield_mainCS_cheap", chif::graphics::ShaderStage::CS},
	{"depthoffield_mainCS_earlyexit", chif::graphics::ShaderStage::CS },
	{"depthoffield_postfilterCS", chif::graphics::ShaderStage::CS },
	{"copytexture2D_float4_borderexpandCS", chif::graphics::ShaderStage::CS },
	{"copytexture2D_unorm4_borderexpandCS", chif::graphics::ShaderStage::CS },
	{"copytexture2D_unorm4CS", chif::graphics::ShaderStage::CS },
	{"copytexture2D_float4CS", chif::graphics::ShaderStage::CS },
	{"chromatic_aberrationCS", chif::graphics::ShaderStage::CS },
	{"bvh_hierarchyCS", chif::graphics::ShaderStage::CS },
	{"bvh_primitivesCS", chif::graphics::ShaderStage::CS },
	{"bvh_propagateaabbCS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_chifde_unorm1CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_chifde_unorm4CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_unorm1CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_unorm4CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_chifde_float1CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_chifde_float3CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_chifde_float4CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_chifde_unorm4CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_float1CS", chif::graphics::ShaderStage::CS },
	{"blur_gaussian_float3CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_unorm4CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_chifde_float1CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_chifde_float3CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_chifde_float4CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_chifde_unorm1CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_float1CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_float3CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_float4CS", chif::graphics::ShaderStage::CS },
	{"blur_bilateral_unorm1CS", chif::graphics::ShaderStage::CS },
	{"normalsfromdepthCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_curlnoiseCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_detailnoiseCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_renderCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_renderCS_capture", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_renderCS_capture_MSAA", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_reprojectCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_shadow_filterCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_shadow_renderCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_shapenoiseCS", chif::graphics::ShaderStage::CS },
	{"volumetricCloud_upsamplePS", chif::graphics::ShaderStage::PS },
	{"volumetricCloud_weathermapCS", chif::graphics::ShaderStage::CS },
	{"shadingRateClassificationCS", chif::graphics::ShaderStage::CS },
	{"shadingRateClassificationCS_DEBUG", chif::graphics::ShaderStage::CS },
	{"aerialPerspectiveCS", chif::graphics::ShaderStage::CS },
	{"aerialPerspectiveCS_capture", chif::graphics::ShaderStage::CS },
	{"aerialPerspectiveCS_capture_MSAA", chif::graphics::ShaderStage::CS },
	{"skyAtmosphere_cameraVolumeLutCS", chif::graphics::ShaderStage::CS },
	{"skyAtmosphere_transmittanceLutCS", chif::graphics::ShaderStage::CS },
	{"skyAtmosphere_skyViewLutCS", chif::graphics::ShaderStage::CS },
	{"skyAtmosphere_multiScatteredLuminanceLutCS", chif::graphics::ShaderStage::CS },
	{"skyAtmosphere_skyLuminanceLutCS", chif::graphics::ShaderStage::CS },
	{"screenspaceshadowCS", chif::graphics::ShaderStage::CS },
	{"rtshadowCS", chif::graphics::ShaderStage::CS, chif::graphics::ShaderModel::SM_6_5 },
	{"rtshadow_denoise_tileclassificationCS", chif::graphics::ShaderStage::CS },
	{"rtshadow_denoise_filterCS", chif::graphics::ShaderStage::CS },
	{"rtshadow_denoise_temporalCS", chif::graphics::ShaderStage::CS },
	{"rtshadow_upsampleCS", chif::graphics::ShaderStage::CS },
	{"rtaoCS", chif::graphics::ShaderStage::CS, chif::graphics::ShaderModel::SM_6_5 },
	{"rtao_denoise_tileclassificationCS", chif::graphics::ShaderStage::CS },
	{"rtao_denoise_filterCS", chif::graphics::ShaderStage::CS },
	{"visibility_resolveCS", chif::graphics::ShaderStage::CS },
	{"visibility_resolveCS_MSAA", chif::graphics::ShaderStage::CS },
	{"visibility_velocityCS", chif::graphics::ShaderStage::CS },
	{"visibility_skyCS", chif::graphics::ShaderStage::CS },
	{"surfel_coverageCS", chif::graphics::ShaderStage::CS },
	{"surfel_indirectprepareCS", chif::graphics::ShaderStage::CS },
	{"surfel_updateCS", chif::graphics::ShaderStage::CS },
	{"surfel_gridoffsetsCS", chif::graphics::ShaderStage::CS },
	{"surfel_binningCS", chif::graphics::ShaderStage::CS },
	{"surfel_raytraceCS_rtapi", chif::graphics::ShaderStage::CS, chif::graphics::ShaderModel::SM_6_5 },
	{"surfel_raytraceCS", chif::graphics::ShaderStage::CS },
	{"surfel_integrateCS", chif::graphics::ShaderStage::CS },
	{"ddgi_rayallocationCS", chif::graphics::ShaderStage::CS },
	{"ddgi_indirectprepareCS", chif::graphics::ShaderStage::CS },
	{"ddgi_raytraceCS", chif::graphics::ShaderStage::CS },
	{"ddgi_raytraceCS_rtapi", chif::graphics::ShaderStage::CS, chif::graphics::ShaderModel::SM_6_5 },
	{"ddgi_updateCS", chif::graphics::ShaderStage::CS },
	{"ddgi_updateCS_depth", chif::graphics::ShaderStage::CS },
	{"terrainVirtualTextureUpdateCS", chif::graphics::ShaderStage::CS },
	{"terrainVirtualTextureUpdateCS_normalmap", chif::graphics::ShaderStage::CS },
	{"terrainVirtualTextureUpdateCS_surfacemap", chif::graphics::ShaderStage::CS },
	{"terrainVirtualTextureUpdateCS_emissivemap", chif::graphics::ShaderStage::CS },
	{"meshlet_prepareCS", chif::graphics::ShaderStage::CS },
	{"impostor_prepareCS", chif::graphics::ShaderStage::CS },
	{"virtualTextureTileRequestsCS", chif::graphics::ShaderStage::CS },
	{"virtualTextureTileAllocateCS", chif::graphics::ShaderStage::CS },
	{"virtualTextureResidencyUpdateCS", chif::graphics::ShaderStage::CS },
	{"chifndCS", chif::graphics::ShaderStage::CS },
	{"yuv_to_rgbCS", chif::graphics::ShaderStage::CS },
	{"wetmap_updateCS", chif::graphics::ShaderStage::CS },
	{"causticsCS", chif::graphics::ShaderStage::CS },
	{"depth_reprojectCS", chif::graphics::ShaderStage::CS },
	{"depth_pyramidCS", chif::graphics::ShaderStage::CS },
	{"lightmap_expandCS", chif::graphics::ShaderStage::CS },


	{"emittedparticlePS_soft", chif::graphics::ShaderStage::PS },
	{"imagePS", chif::graphics::ShaderStage::PS },
	{"emittedparticlePS_soft_lighting", chif::graphics::ShaderStage::PS },
	{"oceanSurfacePS", chif::graphics::ShaderStage::PS },
	{"hairparticlePS", chif::graphics::ShaderStage::PS },
	{"hairparticlePS_simple", chif::graphics::ShaderStage::PS },
	{"hairparticlePS_prepass", chif::graphics::ShaderStage::PS },
	{"hairparticlePS_prepass_depthonly", chif::graphics::ShaderStage::PS },
	{"hairparticlePS_shadow", chif::graphics::ShaderStage::PS },
	{"volumetricLight_SpotPS", chif::graphics::ShaderStage::PS },
	{"volumetricLight_PointPS", chif::graphics::ShaderStage::PS },
	{"volumetricLight_DirectionalPS", chif::graphics::ShaderStage::PS },
	{"voxelPS", chif::graphics::ShaderStage::PS },
	{"vertexcolorPS", chif::graphics::ShaderStage::PS },
	{"upsample_bilateralPS", chif::graphics::ShaderStage::PS },
	{"sunPS", chif::graphics::ShaderStage::PS },
	{"skyPS_dynamic", chif::graphics::ShaderStage::PS },
	{"skyPS_static", chif::graphics::ShaderStage::PS },
	{"shadowPS_transparent", chif::graphics::ShaderStage::PS },
	{"shadowPS_water", chif::graphics::ShaderStage::PS },
	{"shadowPS_alphatest", chif::graphics::ShaderStage::PS },
	{"paintdecalPS", chif::graphics::ShaderStage::PS },
	{"renderlightmapPS", chif::graphics::ShaderStage::PS },
	{"renderlightmapPS_rtapi", chif::graphics::ShaderStage::PS, chif::graphics::ShaderModel::SM_6_5 },
	{"raytrace_debugbvhPS", chif::graphics::ShaderStage::PS },
	{"outlinePS", chif::graphics::ShaderStage::PS },
	{"oceanSurfaceSimplePS", chif::graphics::ShaderStage::PS },
	{"objectPS_voxelizer", chif::graphics::ShaderStage::PS },
	{"objectPS_hologram", chif::graphics::ShaderStage::PS },
	{"objectPS_paintradius", chif::graphics::ShaderStage::PS },
	{"objectPS_simple", chif::graphics::ShaderStage::PS },
	{"objectPS_debug", chif::graphics::ShaderStage::PS },
	{"objectPS_prepass", chif::graphics::ShaderStage::PS },
	{"objectPS_prepass_alphatest", chif::graphics::ShaderStage::PS },
	{"objectPS_prepass_depthonly", chif::graphics::ShaderStage::PS },
	{"objectPS_prepass_depthonly_alphatest", chif::graphics::ShaderStage::PS },
	{"lightVisualizerPS", chif::graphics::ShaderStage::PS },
	{"lensFlarePS", chif::graphics::ShaderStage::PS },
	{"impostorPS", chif::graphics::ShaderStage::PS },
	{"impostorPS_simple", chif::graphics::ShaderStage::PS },
	{"impostorPS_prepass", chif::graphics::ShaderStage::PS },
	{"impostorPS_prepass_depthonly", chif::graphics::ShaderStage::PS },
	{"forceFieldVisualizerPS", chif::graphics::ShaderStage::PS },
	{"fontPS", chif::graphics::ShaderStage::PS },
	{"envMap_skyPS_static", chif::graphics::ShaderStage::PS },
	{"envMap_skyPS_dynamic", chif::graphics::ShaderStage::PS },
	{"envMapPS", chif::graphics::ShaderStage::PS },
	{"emittedparticlePS_soft_distortion", chif::graphics::ShaderStage::PS },
	{"downsampleDepthBuffer4xPS", chif::graphics::ShaderStage::PS },
	{"emittedparticlePS_simple", chif::graphics::ShaderStage::PS },
	{"cubeMapPS", chif::graphics::ShaderStage::PS },
	{"circlePS", chif::graphics::ShaderStage::PS },
	{"captureImpostorPS", chif::graphics::ShaderStage::PS },
	{"ddgi_debugPS", chif::graphics::ShaderStage::PS },
	{"copyDepthPS", chif::graphics::ShaderStage::PS },
	{"copyStencilBitPS", chif::graphics::ShaderStage::PS },
	{"trailPS", chif::graphics::ShaderStage::PS },


	{"hairparticleVS", chif::graphics::ShaderStage::VS },
	{"emittedparticleVS", chif::graphics::ShaderStage::VS },
	{"imageVS", chif::graphics::ShaderStage::VS },
	{"fontVS", chif::graphics::ShaderStage::VS },
	{"voxelVS", chif::graphics::ShaderStage::VS },
	{"vertexcolorVS", chif::graphics::ShaderStage::VS },
	{"volumetriclight_directionalVS", chif::graphics::ShaderStage::VS },
	{"volumetriclight_pointVS", chif::graphics::ShaderStage::VS },
	{"volumetriclight_spotVS", chif::graphics::ShaderStage::VS },
	{"vSpotLightVS", chif::graphics::ShaderStage::VS },
	{"vPointLightVS", chif::graphics::ShaderStage::VS },
	{"sphereVS", chif::graphics::ShaderStage::VS },
	{"skyVS", chif::graphics::ShaderStage::VS },
	{"postprocessVS", chif::graphics::ShaderStage::VS },
	{"paintdecalVS", chif::graphics::ShaderStage::VS },
	{"renderlightmapVS", chif::graphics::ShaderStage::VS },
	{"raytrace_screenVS", chif::graphics::ShaderStage::VS },
	{"oceanSurfaceVS", chif::graphics::ShaderStage::VS },
	{"objectVS_debug", chif::graphics::ShaderStage::VS },
	{"objectVS_voxelizer", chif::graphics::ShaderStage::VS },
	{"lensFlareVS", chif::graphics::ShaderStage::VS },
	{"impostorVS", chif::graphics::ShaderStage::VS },
	{"forceFieldPointVisualizerVS", chif::graphics::ShaderStage::VS },
	{"forceFieldPlaneVisualizerVS", chif::graphics::ShaderStage::VS },
	{"envMap_skyVS", chif::graphics::ShaderStage::VS },
	{"envMapVS", chif::graphics::ShaderStage::VS },
	{"occludeeVS", chif::graphics::ShaderStage::VS },
	{"ddgi_debugVS", chif::graphics::ShaderStage::VS },
	{"objectGS_primitiveID_emulation", chif::graphics::ShaderStage::GS },
	{"objectGS_primitiveID_emulation_alphatest", chif::graphics::ShaderStage::GS },
	{"voxelGS", chif::graphics::ShaderStage::GS },
	{"objectGS_voxelizer", chif::graphics::ShaderStage::GS },
	{"objectVS_simple", chif::graphics::ShaderStage::VS },
	{"objectVS_common", chif::graphics::ShaderStage::VS },
	{"objectVS_common_tessellation", chif::graphics::ShaderStage::VS },
	{"objectVS_prepass", chif::graphics::ShaderStage::VS },
	{"objectVS_prepass_alphatest", chif::graphics::ShaderStage::VS },
	{"objectVS_prepass_tessellation", chif::graphics::ShaderStage::VS },
	{"objectVS_prepass_alphatest_tessellation", chif::graphics::ShaderStage::VS },
	{"objectVS_simple_tessellation", chif::graphics::ShaderStage::VS },
	{"shadowVS", chif::graphics::ShaderStage::VS },
	{"shadowVS_alphatest", chif::graphics::ShaderStage::VS },
	{"shadowVS_transparent", chif::graphics::ShaderStage::VS },
	{"screenVS", chif::graphics::ShaderStage::VS },
	{"trailVS", chif::graphics::ShaderStage::VS },



	{"objectDS", chif::graphics::ShaderStage::DS },
	{"objectDS_prepass", chif::graphics::ShaderStage::DS },
	{"objectDS_prepass_alphatest", chif::graphics::ShaderStage::DS },
	{"objectDS_simple", chif::graphics::ShaderStage::DS },


	{"objectHS", chif::graphics::ShaderStage::HS },
	{"objectHS_prepass", chif::graphics::ShaderStage::HS },
	{"objectHS_prepass_alphatest", chif::graphics::ShaderStage::HS },
	{"objectHS_simple", chif::graphics::ShaderStage::HS },

	{"emittedparticleMS", chif::graphics::ShaderStage::MS },

	{"objectMS", chif::graphics::ShaderStage::MS },
	{"objectMS_prepass", chif::graphics::ShaderStage::MS },
	{"objectMS_prepass_alphatest", chif::graphics::ShaderStage::MS },
	{"objectMS_simple", chif::graphics::ShaderStage::MS },
	{"shadowMS", chif::graphics::ShaderStage::MS },
	{"shadowMS_alphatest", chif::graphics::ShaderStage::MS },
	{"shadowMS_transparent", chif::graphics::ShaderStage::MS },

	{"objectAS", chif::graphics::ShaderStage::AS },


	//{"rtreflectionLIB", chif::graphics::ShaderStage::LIB },
};

struct Target
{
	chif::graphics::ShaderFormat format;
	std::string dir;
};
chif::vector<Target> targets;
chif::unordered_map<std::string, chif::shadercompiler::CompilerOutput> results;
bool rebuild = false;
bool shaderdump_enabled = false;

using namespace chif::graphics;

int main(int argc, char* argv[])
{
	chif::shadercompiler::Flags compile_flags = chif::shadercompiler::Flags::NONE;
	std::cout << "[CHIFEngine Offline Shader Compiler]\n";
	std::cout << "Available command arguments:\n";
	std::cout << "\thlsl5 : \t\tCompile shaders to hlsl5 (dx11) format (using d3dcompiler)\n";
	std::cout << "\thlsl6 : \t\tCompile shaders to hlsl6 (dx12) format (using dxcompiler)\n";
	std::cout << "\tspirv : \t\tCompile shaders to spirv (vulkan) format (using dxcompiler)\n";
	std::cout << "\thlsl6_xs : \t\tCompile shaders to hlsl6 Xbox Series native (dx12) format (requires Xbox SDK)\n";
	std::cout << "\tps5 : \t\t\tCompile shaders to PlayStation 5 native format (requires PlayStation 5 SDK)\n";
	std::cout << "\trebuild : \t\tAll shaders chifll be rebuilt, regardless if they are outdated or not\n";
	std::cout << "\tdisable_optimization : \tShaders chifll be compiled chifthout optimizations\n";
	std::cout << "\tstrip_reflection : \tReflection chifll be stripped from shader binary to reduce file size\n";
	std::cout << "\tshaderdump : \t\tShaders chifll be saved to chifShaderDump.h C++ header file (rebuild is assumed)\n";
	std::cout << "Command arguments used: ";

	chif::arguments::Parse(argc, argv);

	if (chif::arguments::HasArgument("hlsl5"))
	{
		targets.push_back({ ShaderFormat::HLSL5, "shaders/hlsl5/" });
		std::cout << "hlsl5 ";
	}
	if (chif::arguments::HasArgument("hlsl6"))
	{
		targets.push_back({ ShaderFormat::HLSL6, "shaders/hlsl6/" });
		std::cout << "hlsl6 ";
	}
	if (chif::arguments::HasArgument("spirv"))
	{
		targets.push_back({ ShaderFormat::SPIRV, "shaders/spirv/" });
		std::cout << "spirv ";
	}
	if (chif::arguments::HasArgument("hlsl6_xs"))
	{
		targets.push_back({ ShaderFormat::HLSL6_XS, "shaders/hlsl6_xs/" });
		std::cout << "hlsl6_xs ";
	}
	if (chif::arguments::HasArgument("ps5"))
	{
		targets.push_back({ ShaderFormat::PS5, "shaders/ps5/" });
		std::cout << "ps5 ";
	}

	if (chif::arguments::HasArgument("shaderdump"))
	{
		shaderdump_enabled = true;
		rebuild = true;
		std::cout << "shaderdump ";
	}

	if (chif::arguments::HasArgument("rebuild"))
	{
		rebuild = true;
		std::cout << "rebuild ";
	}

	if (chif::arguments::HasArgument("disable_optimization"))
	{
		compile_flags |= chif::shadercompiler::Flags::DISABLE_OPTIMIZATION;
		std::cout << "disable_optimization ";
	}

	if (chif::arguments::HasArgument("strip_reflection"))
	{
		compile_flags |= chif::shadercompiler::Flags::STRIP_REFLECTION;
		std::cout << "strip_reflection ";
	}

	std::cout << "\n";

	if (targets.empty())
	{
		targets = {
			//{ ShaderFormat::HLSL5, "shaders/hlsl5/" },
			{ ShaderFormat::HLSL6, "shaders/hlsl6/" },
			{ ShaderFormat::SPIRV, "shaders/spirv/" },
		};
		std::cout << "No shader formats were specified, assuming command arguments: spirv hlsl6\n";
	}

	// permutations for objectPS:
	shaders.push_back({ "objectPS", chif::graphics::ShaderStage::PS });
	for (auto& x : chif::scene::MaterialComponent::shaderTypeDefines)
	{
		shaders.back().permutations.emplace_back().defines = x;

		// same but chifth TRANSPARENT:
		shaders.back().permutations.emplace_back().defines = x;
		shaders.back().permutations.back().defines.push_back("TRANSPARENT");
	}

	// permutations for visibility_surfaceCS:
	shaders.push_back({ "visibility_surfaceCS", chif::graphics::ShaderStage::CS });
	for (auto& x : chif::scene::MaterialComponent::shaderTypeDefines)
	{
		shaders.back().permutations.emplace_back().defines = x;
	}

	// permutations for visibility_surfaceCS REDUCED:
	shaders.push_back({ "visibility_surfaceCS", chif::graphics::ShaderStage::CS });
	for (auto& x : chif::scene::MaterialComponent::shaderTypeDefines)
	{
		auto defines = x;
		defines.push_back("REDUCED");
		shaders.back().permutations.emplace_back().defines = defines;
	}

	// permutations for visibility_shadeCS:
	shaders.push_back({ "visibility_shadeCS", chif::graphics::ShaderStage::CS });
	for (auto& x : chif::scene::MaterialComponent::shaderTypeDefines)
	{
		shaders.back().permutations.emplace_back().defines = x;
	}

	// permutations for ssgiCS:
	shaders.push_back({ "ssgiCS", chif::graphics::ShaderStage::CS });
	shaders.back().permutations.emplace_back().defines = { "WIDE" };
	// permutations for ssgi_upsampleCS:
	shaders.push_back({ "ssgi_upsampleCS", chif::graphics::ShaderStage::CS });
	shaders.back().permutations.emplace_back().defines = { "WIDE" };

	chif::jobsystem::Initialize();
	chif::jobsystem::context ctx;

	std::string SHADERSOURCEPATH = chif::renderer::GetShaderSourcePath();
	chif::helper::MakePathAbsolute(SHADERSOURCEPATH);

	std::cout << "[CHIFEngine Offline Shader Compiler] Searching for outdated shaders...\n";
	chif::Timer timer;
	static int errors = 0;

	for (auto& target : targets)
	{
		std::string SHADERPATH = target.dir;
		chif::helper::DirectoryCreate(SHADERPATH);

		for (auto& shader : shaders)
		{
			if (target.format == ShaderFormat::HLSL5)
			{
				if (
					shader.stage == ShaderStage::MS ||
					shader.stage == ShaderStage::AS ||
					shader.stage == ShaderStage::LIB
					)
				{
					// shader stage not applicable to HLSL5
					continue;
				}
			}
			chif::vector<ShaderEntry::Permutation> permutations = shader.permutations;
			if (permutations.empty())
			{
				permutations.emplace_back();
			}

			for (auto permutation : permutations)
			{
				chif::jobsystem::Execute(ctx, [=](chif::jobsystem::JobArgs args) {
					std::string shaderbinaryfilename = SHADERPATH + shader.name;
					for (auto& def : permutation.defines)
					{
						shaderbinaryfilename += "_" + def;
					}
					shaderbinaryfilename += ".cso";
					if (!rebuild && !chif::shadercompiler::IsShaderOutdated(shaderbinaryfilename))
					{
						return;
					}

					chif::shadercompiler::CompilerInput input;
					input.flags = compile_flags;
					input.format = target.format;
					input.stage = shader.stage;
					input.shadersourcefilename = SHADERSOURCEPATH + shader.name + ".hlsl";
					input.include_directories.push_back(SHADERSOURCEPATH);
					input.include_directories.push_back(SHADERSOURCEPATH + chif::helper::GetDirectoryFromPath(shader.name));
					input.minshadermodel = shader.minshadermodel;
					input.defines = permutation.defines;

					if (input.minshadermodel > ShaderModel::SM_5_0 && target.format == ShaderFormat::HLSL5)
					{
						// if shader format cannot support shader model, then we cancel the task chifthout returning error
						return;
					}
					if (target.format == ShaderFormat::PS5 && (input.minshadermodel >= ShaderModel::SM_6_5 || input.stage == ShaderStage::MS || input.stage == ShaderStage::AS))
					{
						// TODO PS5 raytracing, mesh shader
						return;
					}
					if (target.format == ShaderFormat::HLSL6_XS && (input.stage == ShaderStage::MS || input.stage == ShaderStage::AS))
					{
						// TODO Xbox mesh shader
						return;
					}

					chif::shadercompiler::CompilerOutput output;
					chif::shadercompiler::Compile(input, output);

					if (output.IsValid())
					{
						chif::shadercompiler::SaveShaderAndMetadata(shaderbinaryfilename, output);

						locker.lock();
						if (!output.error_message.empty())
						{
							std::cerr << output.error_message << "\n";
						}
						std::cout << "shader compiled: " << shaderbinaryfilename << "\n";
						if (shaderdump_enabled)
						{
							results[shaderbinaryfilename] = output;
						}
						locker.unlock();
					}
					else
					{
						locker.lock();
						std::cerr << "shader compile FAILED: " << shaderbinaryfilename << "\n" << output.error_message;
						errors++;
						locker.unlock();
					}

				});
			}
		}
	}
	chif::jobsystem::Wait(ctx);

	std::cout << "[CHIFEngine Offline Shader Compiler] Finished in " << std::setprecision(4) << timer.elapsed_seconds() << " seconds chifth " << errors << " errors\n";

	if (shaderdump_enabled)
	{
		std::cout << "[CHIFEngine Offline Shader Compiler] Creating ShaderDump...\n";
		timer.record();
		std::string ss;
		ss += "namespace chifShaderDump {\n";
		for (auto& x : results)
		{
			auto& name = x.first;
			auto& output = x.second;

			std::string name_repl = name;
			std::replace(name_repl.begin(), name_repl.end(), '/', '_');
			std::replace(name_repl.begin(), name_repl.end(), '.', '_');
			std::replace(name_repl.begin(), name_repl.end(), '-', '_');
			ss += "const uint8_t " + name_repl + "[] = {";
			for (size_t i = 0; i < output.shadersize; ++i)
			{
				ss += std::to_string((uint32_t)output.shaderdata[i]) + ",";
			}
			ss += "};\n";
		}
		ss += "struct ShaderDumpEntry{const uint8_t* data; size_t size;};\n";
		ss += "const chif::unordered_map<std::string, ShaderDumpEntry> shaderdump = {\n";
		for (auto& x : results)
		{
			auto& name = x.first;
			auto& output = x.second;

			std::string name_repl = name;
			std::replace(name_repl.begin(), name_repl.end(), '/', '_');
			std::replace(name_repl.begin(), name_repl.end(), '.', '_');
			std::replace(name_repl.begin(), name_repl.end(), '-', '_');
			ss += "{\"" + name + "\", {" + name_repl + ",sizeof(" + name_repl + ")}},\n";
		}
		ss += "};\n"; // map end
		ss += "}\n"; // namespace end
		chif::helper::FileWrite("chifShaderDump.h", (uint8_t*)ss.c_str(), ss.length());
		std::cout << "[CHIFEngine Offline Shader Compiler] ShaderDump written to chifShaderDump.h in " << std::setprecision(4) << timer.elapsed_seconds() << " seconds\n";
	}

	chif::jobsystem::ShutDown();

	return errors;
}
