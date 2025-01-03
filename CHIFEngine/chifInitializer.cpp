#include "chifInitializer.h"
#include "CHIFEngine.h"

#include <string>
#include <thread>
#include <atomic>

namespace chif::initializer
{
	static std::atomic_bool initializationStarted{ false };
	static chif::jobsystem::context ctx;
	static chif::Timer timer;
	static std::atomic_bool systems[INITIALIZED_SYSTEM_COUNT]{};

	void InitializeComponentsImmediate()
	{
		if (IsInitializeFinished())
			return;
		if (!initializationStarted.load())
		{
			InitializeComponentsAsync();
		}
		WaitForInitializationsToFinish();
	}
	void InitializeComponentsAsync()
	{
		if (IsInitializeFinished())
			return;
		timer.record();

		initializationStarted.store(true);

		std::string ss;
		ss += "\n[chif::initializer] Initializing CHIFEngine, please wait...\n";
		ss += "Version: ";
		ss += chif::version::GetVersionString();
		chif::backlog::post(ss);

		size_t shaderdump_count = chif::renderer::GetShaderDumpCount();
		if (shaderdump_count > 0)
		{
			chif::backlog::post("\nEmbedded shaders found: " + std::to_string(shaderdump_count));
		}
		else
		{
			chif::backlog::post("\nNo embedded shaders found, shaders will be compiled at runtime if needed.\n\tShader source path: " + chif::renderer::GetShaderSourcePath() + "\n\tShader binary path: " + chif::renderer::GetShaderPath());
		}

		chif::backlog::post("");
		chif::jobsystem::Initialize();

		chif::backlog::post("");
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::font::Initialize(); systems[INITIALIZED_SYSTEM_FONT].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::image::Initialize(); systems[INITIALIZED_SYSTEM_IMAGE].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::input::Initialize(); systems[INITIALIZED_SYSTEM_INPUT].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::renderer::Initialize(); systems[INITIALIZED_SYSTEM_RENDERER].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::texturehelper::Initialize(); systems[INITIALIZED_SYSTEM_TEXTUREHELPER].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::HairParticleSystem::Initialize(); systems[INITIALIZED_SYSTEM_HAIRPARTICLESYSTEM].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::EmittedParticleSystem::Initialize(); systems[INITIALIZED_SYSTEM_EMITTEDPARTICLESYSTEM].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::Ocean::Initialize(); systems[INITIALIZED_SYSTEM_OCEAN].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::gpusortlib::Initialize(); systems[INITIALIZED_SYSTEM_GPUSORTLIB].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::GPUBVH::Initialize(); systems[INITIALIZED_SYSTEM_GPUBVH].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::physics::Initialize(); systems[INITIALIZED_SYSTEM_PHYSICS].store(true); });
		chif::jobsystem::Execute(ctx, [](chif::jobsystem::JobArgs args) { chif::TrailRenderer::Initialize(); systems[INITIALIZED_SYSTEM_TRAILRENDERER].store(true); });

		// Initialize these immediately:
		chif::lua::Initialize(); systems[INITIALIZED_SYSTEM_LUA].store(true);
		chif::audio::Initialize(); systems[INITIALIZED_SYSTEM_AUDIO].store(true);

		std::thread([] {
			chif::jobsystem::Wait(ctx);
			chif::backlog::post("\n[chif::initializer] CHIFEngine Initialized (" + std::to_string((int)std::round(timer.elapsed())) + " ms)");
		}).detach();

	}

	bool IsInitializeFinished(INITIALIZED_SYSTEM system)
	{
		if (system == INITIALIZED_SYSTEM_COUNT)
		{
			return initializationStarted.load() && !chif::jobsystem::IsBusy(ctx);
		}
		else
		{
			return systems[system].load();
		}
	}

	void WaitForInitializationsToFinish()
	{
		chif::jobsystem::Wait(ctx);
	}
}
