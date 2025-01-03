#include "chifRenderer_BindLua.h"
#include "chifRenderer.h"
#include "chifHelper.h"
#include "chifScene.h"
#include "chifScene_BindLua.h"
#include "chifMath_BindLua.h"
#include "chifTexture_BindLua.h"
#include "chifEmittedParticle.h"
#include "chifHairParticle.h"
#include "chifPrimitive_BindLua.h"
#include "chifEventHandler.h"
#include "chifVoxelGrid_BindLua.h"
#include "chifPathQuery_BindLua.h"
#include "chifTrailRenderer_BindLua.h"

using namespace chif::ecs;
using namespace chif::graphics;
using namespace chif::scene;
using namespace chif::lua::scene;
using namespace chif::lua::primitive;

namespace chif::lua::renderer
{
	int SetGamma(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::lua::SSetString(L, "SetGamma() no longer supported!");
		}
		else
		{
			chif::lua::SError(L, "SetGamma(float) not enough arguments!");
		}
		return 0;
	}
	int SetGameSpeed(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetGameSpeed(chif::lua::SGetFloat(L, 1));
		}
		else
		{
			chif::lua::SError(L,"SetGameSpeed(float) not enough arguments!");
		}
		return 0;
	}
	int GetGameSpeed(lua_State* L)
	{
		chif::lua::SSetFloat(L, chif::renderer::GetGameSpeed());
		return 1;
	}
	int IsRaytracingSupported(lua_State* L)
	{
		chif::lua::SSetBool(L, chif::graphics::GetDevice()->CheckCapability(GraphicsDeviceCapability::RAYTRACING));
		return 1;
	}

	int SetShadowProps2D(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetShadowProps2D(chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "SetShadowProps2D(int max_resolution) not enough arguments!");
		return 0;
	}
	int SetShadowPropsCube(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetShadowPropsCube(chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "SetShadowPropsCube(int max_resolution) not enough arguments!");
		return 0;
	}
	int SetDebugPartitionTreeEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawDebugPartitionTree(chif::lua::SGetBool(L, 1));
		}
		return 0;
	}
	int SetDebugBoxesEnabled(lua_State* L)
	{
		chif::lua::SError(L, "SetDebugBoxesEnabled is obsolete! Use SetDebugPartitionTreeEnabled(bool value) instead to draw a partition tree!");
		return 0;
	}
	int SetDebugBonesEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawDebugBoneLines(chif::lua::SGetBool(L, 1));
		}
		return 0;
	}
	int SetDebugEmittersEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawDebugEmitters(chif::lua::SGetBool(L, 1));
		}
		return 0;
	}
	int SetDebugEnvProbesEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawDebugEnvProbes(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetDebugEnvProbesEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetDebugForceFieldsEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawDebugForceFields(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetDebugForceFieldsEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetDebugCamerasEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawDebugCameras(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetDebugCamerasEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetDebugCollidersEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawDebugColliders(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetDebugCollidersEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetGridHelperEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetToDrawGridHelper(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetGridHelperEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetDDGIDebugEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetDDGIDebugEnabled(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetDDGIDebugEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetVSyncEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::eventhandler::SetVSync(chif::lua::SGetBool(L, 1));
		}
		return 0;
	}
	int SetResolution(lua_State* L)
	{
		chif::lua::SError(L, "SetResolution() is deprecated, now it's handled by window events!");
		return 0;
	}
	int SetDebugLightCulling(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetDebugLightCulling(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetDebugLightCulling(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetOcclusionCullingEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetOcclusionCullingEnabled(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetOcclusionCullingEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetTemporalAAEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetTemporalAAEnabled(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetTemporalAAEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetRaytracedShadowsEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetRaytracedShadowsEnabled(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetRaytracedShadowsEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetMeshShaderAllowed(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetMeshShaderAllowed(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetMeshShaderAllowed(bool enabled) not enough arguments!");
		}
		return 0;
	}
	int SetMeshletOcclusionCullingEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::renderer::SetMeshletOcclusionCullingEnabled(chif::lua::SGetBool(L, 1));
		}
		else
		{
			chif::lua::SError(L, "SetMeshletOcclusionCullingEnabled(bool enabled) not enough arguments!");
		}
		return 0;
	}

	int DrawLine(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			Vector_BindLua* a = Luna<Vector_BindLua>::lightcheck(L, 1);
			Vector_BindLua* b = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (a && b)
			{
				chif::renderer::RenderableLine line;
				XMStoreFloat3(&line.start, XMLoadFloat4(&a->data));
				XMStoreFloat3(&line.end, XMLoadFloat4(&b->data));
				bool depth = false;
				if (argc > 2)
				{
					Vector_BindLua* c = Luna<Vector_BindLua>::lightcheck(L, 3);
					if (c)
					{
						XMStoreFloat4(&line.color_start, XMLoadFloat4(&c->data));
						XMStoreFloat4(&line.color_end, XMLoadFloat4(&c->data));
					}
					else
						chif::lua::SError(L, "DrawLine(Vector origin,end, opt Vector color, opt bool depth = false) one or more arguments are not vectors!");

					if (argc > 3)
					{
						depth = chif::lua::SGetBool(L, 4);
					}
				}
				chif::renderer::DrawLine(line, depth);
			}
			else
				chif::lua::SError(L, "DrawLine(Vector origin,end, opt Vector color, opt bool depth = false) one or more arguments are not vectors!");
		}
		else
			chif::lua::SError(L, "DrawLine(Vector origin,end, opt Vector color, opt bool depth = false) not enough arguments!");

		return 0;
	}
	int DrawPoint(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			Vector_BindLua* a = Luna<Vector_BindLua>::lightcheck(L, 1);
			if (a)
			{
				chif::renderer::RenderablePoint point;
				XMStoreFloat3(&point.position, XMLoadFloat4(&a->data));
				bool depth = false;
				if (argc > 1)
				{
					point.size = chif::lua::SGetFloat(L, 2);

					if (argc > 2)
					{
						Vector_BindLua* color = Luna<Vector_BindLua>::lightcheck(L, 3);
						if (color)
						{
							point.color = color->data;
						}

						if (argc > 3)
						{
							depth = chif::lua::SGetBool(L, 4);
						}
					}
				}
				chif::renderer::DrawPoint(point, depth);
			}
			else
				chif::lua::SError(L, "DrawPoint(Vector origin, opt float size, opt Vector color, opt bool depth = false) first argument must be a Vector type!");
		}
		else
			chif::lua::SError(L, "DrawPoint(Vector origin, opt float size, opt Vector color, opt bool depth = false) not enough arguments!");

		return 0;
	}
	int DrawBox(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			Matrix_BindLua* m = Luna<Matrix_BindLua>::lightcheck(L, 1);
			if (m)
			{
				if (argc > 1)
				{
					Vector_BindLua* color = Luna<Vector_BindLua>::lightcheck(L, 2);
					if (color)
					{
						bool depth = true;
						if (argc > 2)
						{
							depth = chif::lua::SGetBool(L, 3);
						}
						chif::renderer::DrawBox(m->data, color->data, depth);
						return 0;
					}
				}

				chif::renderer::DrawBox(m->data);
			}
			else
				chif::lua::SError(L, "DrawBox(Matrix boxMatrix, opt Vector color, opt bool depth = true) first argument must be a Matrix type!");
		}
		else
			chif::lua::SError(L, "DrawBox(Matrix boxMatrix, opt Vector color, opt bool depth = true) not enough arguments!");

		return 0;
	}
	int DrawSphere(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			Sphere_BindLua* sphere = Luna<Sphere_BindLua>::lightcheck(L, 1);
			if (sphere)
			{
				if (argc > 1)
				{
					Vector_BindLua* color = Luna<Vector_BindLua>::lightcheck(L, 2);
					if (color)
					{
						bool depth = true;
						if (argc > 2)
						{
							depth = chif::lua::SGetBool(L, 3);
						}
						chif::renderer::DrawSphere(sphere->sphere, color->data, depth);
						return 0;
					}
				}

				chif::renderer::DrawSphere(sphere->sphere);
			}
			else
				chif::lua::SError(L, "DrawSphere(Sphere sphere, opt Vector color, opt bool depth = true) first argument must be a Matrix type!");
		}
		else
			chif::lua::SError(L, "DrawSphere(Sphere sphere, opt Vector color, opt bool depth = true) not enough arguments!");

		return 0;
	}
	int DrawCapsule(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			Capsule_BindLua* capsule = Luna<Capsule_BindLua>::lightcheck(L, 1);
			if (capsule)
			{
				if (argc > 1)
				{
					Vector_BindLua* color = Luna<Vector_BindLua>::lightcheck(L, 2);
					if (color)
					{
						bool depth = true;
						if (argc > 2)
						{
							depth = chif::lua::SGetBool(L, 3);
						}
						chif::renderer::DrawCapsule(capsule->capsule, color->data, depth);
						return 0;
					}
				}

				chif::renderer::DrawCapsule(capsule->capsule);
			}
			else
				chif::lua::SError(L, "DrawCapsule(Capsule capsule, opt Vector color, opt bool depth = true) first argument must be a Matrix type!");
		}
		else
			chif::lua::SError(L, "DrawCapsule(Capsule capsule, opt Vector color, opt bool depth = true) not enough arguments!");

		return 0;
	}
	int DrawDebugText(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			std::string text = chif::lua::SGetString(L, 1);
			chif::renderer::DebugTextParams params;
			if (argc > 1)
			{
				Vector_BindLua* position = Luna<Vector_BindLua>::lightcheck(L, 2);
				if (position != nullptr)
				{
					params.position.x = position->data.x;
					params.position.y = position->data.y;
					params.position.z = position->data.z;

					if (argc > 2)
					{
						Vector_BindLua* color = Luna<Vector_BindLua>::lightcheck(L, 3);
						if (color != nullptr)
						{
							params.color = color->data;

							if (argc > 3)
							{
								params.scaling = chif::lua::SGetFloat(L, 4);

								if (argc > 4)
								{
									params.flags = chif::lua::SGetInt(L, 5);
								}
							}
						}
						else
							chif::lua::SError(L, "DrawDebugText(string text, opt Vector position, opt Vector color, opt float scaling, opt int flags) third argument was not a Vector!");
					}
				}
				else
					chif::lua::SError(L, "DrawDebugText(string text, opt Vector position, opt Vector color, opt float scaling, opt int flags) second argument was not a Vector!");

			}
			chif::renderer::DrawDebugText(text.c_str(), params);
		}
		else
			chif::lua::SError(L, "DrawDebugText(string text, opt Vector position, opt Vector color, opt float scaling, opt int flags) not enough arguments!");

		return 0;
	}
	int DrawVoxelGrid(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			VoxelGrid_BindLua* a = Luna<VoxelGrid_BindLua>::lightcheck(L, 1);
			if (a)
			{
				chif::renderer::DrawVoxelGrid(a->voxelgrid);
			}
			else
				chif::lua::SError(L, "DrawVoxelGrid(VoxelGrid voxelgrid) first argument must be a VoxelGrid type!");
		}
		else
			chif::lua::SError(L, "DrawVoxelGrid(VoxelGrid voxelgrid) not enough arguments!");

		return 0;
	}
	int DrawPathQuery(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			PathQuery_BindLua* a = Luna<PathQuery_BindLua>::lightcheck(L, 1);
			if (a)
			{
				chif::renderer::DrawPathQuery(a->pathquery);
			}
			else
				chif::lua::SError(L, "DrawPathQuery(PathQuery pathquery) first argument must be a PathQuery type!");
		}
		else
			chif::lua::SError(L, "DrawPathQuery(PathQuery pathquery) not enough arguments!");

		return 0;
	}
	int DrawTrail(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			TrailRenderer_BindLua* a = Luna<TrailRenderer_BindLua>::lightcheck(L, 1);
			if (a)
			{
				chif::renderer::DrawTrail(&a->trail);
			}
			else
				chif::lua::SError(L, "DrawTrail(TrailRenderer trail) first argument must be a TrailRenderer type!");
		}
		else
			chif::lua::SError(L, "DrawTrail(TrailRenderer trail) not enough arguments!");

		return 0;
	}

	class PaintTextureParams_BindLua
	{
	public:
		chif::renderer::PaintTextureParams params;

		PaintTextureParams_BindLua(const chif::renderer::PaintTextureParams& params) : params(params) {}
		PaintTextureParams_BindLua(lua_State* L) {}

		int SetEditTexture(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetEditTexture(Texture tex): not enough arguments!");
				return 0;
			}
			Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
			if (tex == nullptr)
			{
				chif::lua::SError(L, "SetEditTexture(Texture tex): argument is not a Texture!");
				return 0;
			}
			if (tex->resource.IsValid())
			{
				params.editTex = tex->resource.GetTexture();
			}
			return 0;
		}
		int SetBrushTexture(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetBrushTexture(Texture tex): not enough arguments!");
				return 0;
			}
			Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
			if (tex == nullptr)
			{
				chif::lua::SError(L, "SetBrushTexture(Texture tex): argument is not a Texture!");
				return 0;
			}
			if (tex->resource.IsValid())
			{
				params.brushTex = tex->resource.GetTexture();
			}
			return 0;
		}
		int SetRevealTexture(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetRevealTexture(Texture tex): not enough arguments!");
				return 0;
			}
			Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
			if (tex == nullptr)
			{
				chif::lua::SError(L, "SetRevealTexture(Texture tex): argument is not a Texture!");
				return 0;
			}
			if (tex->resource.IsValid())
			{
				params.revealTex = tex->resource.GetTexture();
			}
			return 0;
		}
		int SetCenterPixel(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetCenterPixel(Vector value): not enough arguments!");
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "SetCenterPixel(Vector value): argument is not a Vector!");
				return 0;
			}
			params.push.xPaintBrushCenter.x = uint32_t(vec->data.x);
			params.push.xPaintBrushCenter.y = uint32_t(vec->data.y);
			return 0;
		}
		int SetBrushColor(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetBrushColor(Vector value): not enough arguments!");
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "SetBrushColor(Vector value): argument is not a Vector!");
				return 0;
			}
			params.push.xPaintBrushColor = chif::Color::fromFloat4(vec->data);
			return 0;
		}
		int SetBrushRadius(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetBrushRadius(int value): not enough arguments!");
				return 0;
			}
			params.push.xPaintBrushRadius = chif::lua::SGetInt(L, 1);
			return 0;
		}
		int SetBrushAmount(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetBrushAmount(float value): not enough arguments!");
				return 0;
			}
			params.push.xPaintBrushAmount = chif::lua::SGetFloat(L, 1);
			return 0;
		}
		int SetBrushSmoothness(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetBrushSmoothness(float value): not enough arguments!");
				return 0;
			}
			params.push.xPaintBrushSmoothness = chif::lua::SGetFloat(L, 1);
			return 0;
		}
		int SetBrushRotation(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetBrushRotation(float value): not enough arguments!");
				return 0;
			}
			params.push.xPaintBrushRotation = chif::lua::SGetFloat(L, 1);
			return 0;
		}
		int SetBrushShape(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetBrushShape(float value): not enough arguments!");
				return 0;
			}
			params.push.xPaintBrushShape = chif::lua::SGetInt(L, 1);
			return 0;
		}

		inline static constexpr char className[] = "PaintTextureParams";
		inline static constexpr Luna<PaintTextureParams_BindLua>::FunctionType methods[] = {
			lunamethod(PaintTextureParams_BindLua, SetEditTexture),
			lunamethod(PaintTextureParams_BindLua, SetBrushTexture),
			lunamethod(PaintTextureParams_BindLua, SetRevealTexture),
			lunamethod(PaintTextureParams_BindLua, SetBrushColor),
			lunamethod(PaintTextureParams_BindLua, SetCenterPixel),
			lunamethod(PaintTextureParams_BindLua, SetBrushRadius),
			lunamethod(PaintTextureParams_BindLua, SetBrushAmount),
			lunamethod(PaintTextureParams_BindLua, SetBrushSmoothness),
			lunamethod(PaintTextureParams_BindLua, SetBrushRotation),
			lunamethod(PaintTextureParams_BindLua, SetBrushShape),
			{ nullptr, nullptr }
		};
		inline static constexpr Luna<PaintTextureParams_BindLua>::PropertyType properties[] = {
			{ nullptr, nullptr }
		};
	};

	int PaintIntoTexture(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "PaintIntoTexture(PaintTextureParams params): not enough arguments!");
			return 0;
		}
		PaintTextureParams_BindLua* params = Luna<PaintTextureParams_BindLua>::lightcheck(L, 1);
		if (params == nullptr)
		{
			chif::lua::SError(L, "PaintIntoTexture(PaintTextureParams params): argument is not a PaintTextureParams!");
			return 0;
		}
		chif::renderer::PaintIntoTexture(params->params);
		return 0;
	}
	int CreatePaintableTexture(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 2)
		{
			chif::lua::SError(L, "CreatePaintableTexture(int width,height, opt int mips = 0, opt Vector initialColor = Vector()): not enough arguments!");
			return 0;
		}
		uint32_t width = (uint32_t)chif::lua::SGetInt(L, 1);
		uint32_t height = (uint32_t)chif::lua::SGetInt(L, 2);
		uint32_t mips = 0;
		chif::Color color = chif::Color::Transparent();
		if (argc > 2)
		{
			mips = (uint32_t)chif::lua::SGetInt(L, 3);
			if (argc > 3)
			{
				Vector_BindLua* v = Luna<Vector_BindLua>::lightcheck(L, 4);
				if (v != nullptr)
				{
					color = chif::Color::fromFloat4(v->data);
				}
			}
		}
		Luna<Texture_BindLua>::push(L, chif::renderer::CreatePaintableTexture(width, height, mips, color));
		return 1;
	}


	class PaintDecalParams_BindLua
	{
	public:
		chif::renderer::PaintDecalParams params;

		PaintDecalParams_BindLua(const chif::renderer::PaintDecalParams& params) : params(params) {}
		PaintDecalParams_BindLua(lua_State* L) {}

		int SetInTexture(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetInTexture(Texture tex): not enough arguments!");
				return 0;
			}
			Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
			if (tex == nullptr)
			{
				chif::lua::SError(L, "SetInTexture(Texture tex): argument is not a Texture!");
				return 0;
			}
			if (tex->resource.IsValid())
			{
				params.in_texture = tex->resource.GetTexture();
			}
			return 0;
		}
		int SetOutTexture(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetOutTexture(Texture tex): not enough arguments!");
				return 0;
			}
			Texture_BindLua* tex = Luna<Texture_BindLua>::lightcheck(L, 1);
			if (tex == nullptr)
			{
				chif::lua::SError(L, "SetOutTexture(Texture tex): argument is not a Texture!");
				return 0;
			}
			if (tex->resource.IsValid())
			{
				params.out_texture = tex->resource.GetTexture();
			}
			return 0;
		}
		int SetMatrix(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetMatrix(Matrix mat): not enough arguments!");
				return 0;
			}
			Matrix_BindLua* mat = Luna<Matrix_BindLua>::lightcheck(L, 1);
			if (mat == nullptr)
			{
				chif::lua::SError(L, "SetMatrix(Texture mat): argument is not a Texture!");
				return 0;
			}
			params.decalMatrix = mat->data;
			return 0;
		}
		int SetObject(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetObject(Entity entity): not enough arguments!");
				return 0;
			}
			params.objectEntity = (chif::ecs::Entity)chif::lua::SGetLongLong(L, 1);
			return 0;
		}
		int SetSlopeBlendPower(lua_State* L)
		{
			int argc = chif::lua::SGetArgCount(L);
			if (argc < 1)
			{
				chif::lua::SError(L, "SetSlopeBlendPower(float power): not enough arguments!");
				return 0;
			}
			params.slopeBlendPower = chif::lua::SGetFloat(L, 1);
			return 0;
		}

		inline static constexpr char className[] = "PaintDecalParams";
		inline static constexpr Luna<PaintDecalParams_BindLua>::FunctionType methods[] = {
			lunamethod(PaintDecalParams_BindLua, SetInTexture),
			lunamethod(PaintDecalParams_BindLua, SetOutTexture),
			lunamethod(PaintDecalParams_BindLua, SetMatrix),
			lunamethod(PaintDecalParams_BindLua, SetObject),
			lunamethod(PaintDecalParams_BindLua, SetSlopeBlendPower),
			{ nullptr, nullptr }
		};
		inline static constexpr Luna<PaintDecalParams_BindLua>::PropertyType properties[] = {
			{ nullptr, nullptr }
		};
	};
	int PaintDecalIntoObjectSpaceTexture(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "PaintDecalIntoObjectSpaceTexture(PaintDecalParams params): not enough arguments!");
			return 0;
		}
		PaintDecalParams_BindLua* params = Luna<PaintDecalParams_BindLua>::lightcheck(L, 1);
		if (params == nullptr)
		{
			chif::lua::SError(L, "PaintDecalIntoObjectSpaceTexture(PaintDecalParams params): argument is not a PaintDecalParams!");
			return 0;
		}
		chif::renderer::PaintDecalIntoObjectSpaceTexture(params->params);
		return 0;
	}

	int PutWaterRipple(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			std::string name = chif::lua::SGetString(L, 1);
			Vector_BindLua* v = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (v)
			{
				GetGlobalScene()->PutWaterRipple(name, v->GetFloat3());
			}
			else
				chif::lua::SError(L, "PutWaterRipple(string imagename, Vector position) argument is not a Vector!");
		}
		else if (argc > 0)
		{
			Vector_BindLua* v = Luna<Vector_BindLua>::lightcheck(L, 1);
			if (v)
			{
				GetGlobalScene()->PutWaterRipple(v->GetFloat3());
			}
			else
				chif::lua::SError(L, "PutWaterRipple(Vector position) argument is not a Vector!");
		}
		else
		{
			chif::lua::SError(L, "PutWaterRipple(Vector position) not enough arguments!");
			chif::lua::SError(L, "PutWaterRipple(string imagename, Vector position) not enough arguments!");
		}
		return 0;
	}

	int ClearWorld(lua_State* L)
	{
		Scene_BindLua* scene = Luna<Scene_BindLua>::lightcheck(L, 1);
		if (scene == nullptr)
		{
			chif::renderer::ClearWorld(*GetGlobalScene());
		}
		else
		{
			chif::renderer::ClearWorld(*scene->scene);
		}
		return 0;
	}
	int ReloadShaders(lua_State* L)
	{
		chif::renderer::ReloadShaders();
		return 0;
	}

	void Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;

			Luna<PaintTextureParams_BindLua>::Register(chif::lua::GetLuaState());
			Luna<PaintDecalParams_BindLua>::Register(chif::lua::GetLuaState());

			chif::lua::RegisterFunc("SetGamma", SetGamma);
			chif::lua::RegisterFunc("SetGameSpeed", SetGameSpeed);
			chif::lua::RegisterFunc("GetGameSpeed", GetGameSpeed);
			chif::lua::RegisterFunc("IsRaytracingSupported", IsRaytracingSupported);

			chif::lua::RegisterFunc("SetShadowProps2D", SetShadowProps2D);
			chif::lua::RegisterFunc("SetShadowPropsCube", SetShadowPropsCube);
			chif::lua::RegisterFunc("SetDebugBoxesEnabled", SetDebugBoxesEnabled);
			chif::lua::RegisterFunc("SetDebugPartitionTreeEnabled", SetDebugPartitionTreeEnabled);
			chif::lua::RegisterFunc("SetDebugBonesEnabled", SetDebugBonesEnabled);
			chif::lua::RegisterFunc("SetDebugEmittersEnabled", SetDebugEmittersEnabled);
			chif::lua::RegisterFunc("SetDebugEnvProbesEnabled", SetDebugEnvProbesEnabled);
			chif::lua::RegisterFunc("SetDebugForceFieldsEnabled", SetDebugForceFieldsEnabled);
			chif::lua::RegisterFunc("SetDebugCamerasEnabled", SetDebugCamerasEnabled);
			chif::lua::RegisterFunc("SetDebugCollidersEnabled", SetDebugCollidersEnabled);
			chif::lua::RegisterFunc("SetGridHelperEnabled", SetGridHelperEnabled);
			chif::lua::RegisterFunc("SetDDGIDebugEnabled", SetDDGIDebugEnabled);
			chif::lua::RegisterFunc("SetVSyncEnabled", SetVSyncEnabled);
			chif::lua::RegisterFunc("SetResolution", SetResolution);
			chif::lua::RegisterFunc("SetDebugLightCulling", SetDebugLightCulling);
			chif::lua::RegisterFunc("SetOcclusionCullingEnabled", SetOcclusionCullingEnabled);
			chif::lua::RegisterFunc("SetTemporalAAEnabled", SetTemporalAAEnabled);
			chif::lua::RegisterFunc("SetRaytracedShadowsEnabled", SetRaytracedShadowsEnabled);
			chif::lua::RegisterFunc("SetMeshShaderAllowed", SetMeshShaderAllowed);
			chif::lua::RegisterFunc("SetMeshletOcclusionCullingEnabled", SetMeshletOcclusionCullingEnabled);

			chif::lua::RegisterFunc("DrawLine", DrawLine);
			chif::lua::RegisterFunc("DrawPoint", DrawPoint);
			chif::lua::RegisterFunc("DrawBox", DrawBox);
			chif::lua::RegisterFunc("DrawSphere", DrawSphere);
			chif::lua::RegisterFunc("DrawCapsule", DrawCapsule);
			chif::lua::RegisterFunc("DrawDebugText", DrawDebugText);
			chif::lua::RegisterFunc("DrawVoxelGrid", DrawVoxelGrid);
			chif::lua::RegisterFunc("DrawPathQuery", DrawPathQuery);
			chif::lua::RegisterFunc("DrawTrail", DrawTrail);

			chif::lua::RegisterFunc("PaintIntoTexture", PaintIntoTexture);
			chif::lua::RegisterFunc("CreatePaintableTexture", CreatePaintableTexture);
			chif::lua::RegisterFunc("PaintDecalIntoObjectSpaceTexture", PaintDecalIntoObjectSpaceTexture);

			chif::lua::RegisterFunc("PutWaterRipple", PutWaterRipple);

			chif::lua::RegisterFunc("ClearWorld", ClearWorld);
			chif::lua::RegisterFunc("ReloadShaders", ReloadShaders);

			chif::lua::RunText(R"(
GetScreenWidth = function() return main.GetCanvas().GetLogicalWidth() end
GetScreenHeight = function() return main.GetCanvas().GetLogicalHeight() end

DEBUG_TEXT_DEPTH_TEST = 1
DEBUG_TEXT_CAMERA_FACING = 2
DEBUG_TEXT_CAMERA_SCALING = 4
)");

		}
	}
};
