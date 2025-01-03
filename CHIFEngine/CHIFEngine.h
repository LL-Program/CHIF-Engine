#ifndef CHIFENGINE
#define CHIFENGINE
#define CHIF_ENGINE

// NOTE:
// The purpose of this file is to expose all engine features.
// It should be included in the engine's implementing application not the engine itself!
// It should be included in the precompiled header if available.

#include "CommonInclude.h"

// High-level interface:
#include "chifApplication.h"
#include "chifRenderPath.h"
#include "chifRenderPath2D.h"
#include "chifRenderPath3D.h"
#include "chifRenderPath3D_PathTracing.h"
#include "chifLoadingScreen.h"

// Engine-level systems
#include "chifVersion.h"
#include "chifPlatform.h"
#include "chifBacklog.h"
#include "chifPrimitive.h"
#include "chifImage.h"
#include "chifFont.h"
#include "chifSprite.h"
#include "chifSpriteFont.h"
#include "chifScene.h"
#include "chifECS.h"
#include "chifEmittedParticle.h"
#include "chifHairParticle.h"
#include "chifRenderer.h"
#include "chifMath.h"
#include "chifAudio.h"
#include "chifResourceManager.h"
#include "chifTimer.h"
#include "chifHelper.h"
#include "chifInput.h"
#include "chifRawInput.h"
#include "chifXInput.h"
#include "chifSDLInput.h"
#include "chifTextureHelper.h"
#include "chifRandom.h"
#include "chifColor.h"
#include "chifPhysics.h"
#include "chifEnums.h"
#include "chifInitializer.h"
#include "chifLua.h"
#include "chifGraphics.h"
#include "chifGraphicsDevice.h"
#include "chifGUI.h"
#include "chifArchive.h"
#include "chifSpinLock.h"
#include "chifRectPacker.h"
#include "chifProfiler.h"
#include "chifOcean.h"
#include "chifFFTGenerator.h"
#include "chifArguments.h"
#include "chifGPUBVH.h"
#include "chifGPUSortLib.h"
#include "chifJobSystem.h"
#include "chifNetwork.h"
#include "chifEventHandler.h"
#include "chifShaderCompiler.h"
#include "chifCanvas.h"
#include "chifUnorderedMap.h"
#include "chifUnorderedSet.h"
#include "chifVector.h"
#include "chifNoise.h"
#include "chifConfig.h"
#include "chifTerrain.h"
#include "chifLocalization.h"
#include "chifVideo.h"
#include "chifVoxelGrid.h"
#include "chifPathQuery.h"
#include "chifTrailRenderer.h"

#ifdef PLATFORM_WINDOWS_DESKTOP
#pragma comment(lib,"CHIFEngine_Windows.lib")
#endif // PLATFORM_WINDOWS_DESKTOP

#ifdef PLATFORM_XBOX
#pragma comment(lib,"CHIFEngine_XBOX.lib")
#endif // PLATFORM_XBOX

#ifdef PLATFORM_PS5
#pragma comment(lib,"CHIFEngine_PS5.a")
#endif // PLATFORM_PS5

#ifdef CHIFENGINE_BACKWARDS_COMPATIBILITY_0_59

using namespace chif;
using namespace chif::enums;

namespace chifGraphics = chif::graphics;
namespace chifShaderCompiler = chif::shadercompiler;
namespace chifFFTGenerator = chif::fftgenerator;
namespace chifFont = chif::font;
namespace chifImage = chif::image;
namespace chifGPUSortLib = chif::gpusortlib;
namespace chifRenderer = chif::renderer;
namespace chifTextureHelper = chif::texturehelper;
namespace chifHelper = chif::helper;
namespace chifMath = chif::math;
namespace chifRandom = chif::random;
namespace chifRectPacker = chif::rectpacker;
namespace chifResourceManager = chif::resourcemanager;
namespace chifStartupArguments = chif::arguments;
namespace chifInput = chif::input;
namespace chifXInput = chif::input::xinput;
namespace chifRawInput = chif::input::rawinput;
namespace chifSDLInput = chif::input::sdlinput;
namespace chifAudio = chif::audio;
namespace chifNetwork = chif::network;
namespace chifPhysicsEngine = chif::physics;
namespace chifLua = chif::lua;
namespace chifECS = chif::ecs;
namespace chifEvent = chif::eventhandler;
namespace chifInitializer = chif::initializer;
namespace chifJobSystem = chif::jobsystem;
namespace chifPlatform = chif::platform;
namespace chifScene = chif::scene;
namespace chifBackLog = chif::backlog;
namespace chifProfiler = chif::profiler;
namespace chifVersion = chif::version;

using MainComponent = chif::Application;
using chifFontParams = chif::font::Params;
using chifImageParams = chif::image::Params;
using chifGPUBVH = chif::GPUBVH;
using chifEmittedParticle = chif::EmittedParticleSystem;
using chifHairParticle = chif::HairParticleSystem;
using chifOcean = chif::Ocean;
using chifSprite = chif::Sprite;
using chifSpriteFont = chif::SpriteFont;
using chifGUI = chif::gui::GUI;
using chifEventArgs = chif::gui::EventArgs;
using chifWidget = chif::gui::Widget;
using chifButton = chif::gui::Button;
using chifLabel = chif::gui::Label;
using chifTextInputField = chif::gui::TextInputField;
using chifSlider = chif::gui::Slider;
using chifCheckBox = chif::gui::CheckBox;
using chifComboBox = chif::gui::ComboBox;
using chifWindow = chif::gui::Window;
using chifColorPicker = chif::gui::ColorPicker;
using chifTreeList = chif::gui::TreeList;
using chifArchive = chif::Archive;
using chifColor = chif::Color;
using chifFadeManager = chif::FadeManager;
using chifResource = chif::Resource;
using chifSpinLock = chif::SpinLock;
using chifTimer = chif::Timer;
using chifCanvas = chif::Canvas;
using AABB = chif::primitive::AABB;
using SPHERE = chif::primitive::Sphere;
using CAPSULE = chif::primitive::Capsule;
using RAY = chif::primitive::Ray;
using Frustum = chif::primitive::Frustum;
using Hitbox2D = chif::primitive::Hitbox2D;

using chif::image::STENCILMODE;
using chif::image::STENCILREFMODE;
using chif::image::SAMPLEMODE;
using chif::image::QUALITY;
using chif::font::Alignment;

static constexpr int SYSTEM_EVENT_THREAD_SAFE_POINT = chif::eventhandler::EVENT_THREAD_SAFE_POINT;
static constexpr int SYSTEM_EVENT_RELOAD_SHADERS = chif::eventhandler::EVENT_RELOAD_SHADERS;
static constexpr int SYSTEM_EVENT_SET_VSYNC = chif::eventhandler::EVENT_SET_VSYNC;
static constexpr XMFLOAT4X4 IDENTITYMATRIX = chif::math::IDENTITY_MATRIX;

#endif // CHIFENGINE_BACKWARDS_COMPATIBILITY_0_59



#endif // CHIFENGINE
