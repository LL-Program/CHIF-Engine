#include "chifLoadingScreen.h"
#include "chifApplication.h"
#include "chifEventHandler.h"

#include <thread>

using namespace chif::graphics;

namespace chif
{

	bool LoadingScreen::isActive() const
	{
		return chif::jobsystem::IsBusy(ctx);
	}

	bool LoadingScreen::isFinished() const
	{
		return tasks.empty();
	}

	int LoadingScreen::getProgress() const
	{
		if (launchedTasks == 0)
			return 100;
		uint32_t counter = AtomicLoad(&ctx.counter);
		float percent = 1 - float(counter) / float(launchedTasks);
		return (int)std::round(percent * 100);
	}

	void LoadingScreen::addLoadingFunction(std::function<void(chif::jobsystem::JobArgs)> loadingFunction)
	{
		if (loadingFunction != nullptr)
		{
			tasks.push_back(loadingFunction);
		}
	}

	void LoadingScreen::addLoadingComponent(RenderPath* component, Application* main, float fadeSeconds, chif::Color fadeColor)
	{
		addLoadingFunction([=](chif::jobsystem::JobArgs args) {
			component->Load();
			});
		onFinished([=] {
			main->ActivatePath(component, fadeSeconds, fadeColor);
			});
	}

	void LoadingScreen::onFinished(std::function<void()> finishFunction)
	{
		if (finishFunction != nullptr)
			finish = finishFunction;
	}

	void LoadingScreen::Start()
	{
		launchedTasks = (uint32_t)tasks.size();
		for (auto& x : tasks)
		{
			chif::jobsystem::Execute(ctx, x);
		}
		std::thread([this]() {
			chif::jobsystem::Wait(ctx);
			chif::eventhandler::Subscribe_Once(chif::eventhandler::EVENT_THREAD_SAFE_POINT, [this](uint64_t) {
				if (finish != nullptr)
					finish();
				tasks.clear();
				launchedTasks = 0;
				finish = nullptr;
			});
		}).detach();

		RenderPath2D::Start();
	}

	void LoadingScreen::Compose(chif::graphics::CommandList cmd) const
	{
		if (backgroundTexture.IsValid())
		{
			chif::image::Params fx;
			const Texture& tex = backgroundTexture.GetTexture();
			const TextureDesc& desc = tex.GetDesc();

			const float canvas_aspect = GetLogicalWidth() / GetLogicalHeight();
			const float image_aspect = float(desc.width) / float(desc.height);

			switch (background_mode)
			{
			default:
			case chif::LoadingScreen::BackgroundMode::Fill:
				if (canvas_aspect > image_aspect)
				{
					// display aspect is wider than image:
					fx.siz.x = GetLogicalWidth();
					fx.siz.y = GetLogicalHeight() / image_aspect * canvas_aspect;
				}
				else
				{
					// image aspect is wider or equal to display
					fx.siz.x = GetLogicalWidth() / canvas_aspect * image_aspect;
					fx.siz.y = GetLogicalHeight();
				}
				fx.pos = XMFLOAT3(GetLogicalWidth() * 0.5f, GetLogicalHeight() * 0.5f, 0);
				fx.pivot = XMFLOAT2(0.5f, 0.5f);
				break;
			case chif::LoadingScreen::BackgroundMode::Fit:
				if (canvas_aspect > image_aspect)
				{
					// display aspect is wider than image:
					fx.siz.x = GetLogicalWidth() / canvas_aspect * image_aspect;
					fx.siz.y = GetLogicalHeight();
				}
				else
				{
					// image aspect is wider or equal to display
					fx.siz.x = GetLogicalWidth();
					fx.siz.y = GetLogicalHeight() * canvas_aspect / image_aspect;
				}
				fx.pos = XMFLOAT3(GetLogicalWidth() * 0.5f, GetLogicalHeight() * 0.5f, 0);
				fx.pivot = XMFLOAT2(0.5f, 0.5f);
				break;
			case chif::LoadingScreen::BackgroundMode::Stretch:
				fx.enableFullScreen();
				break;
			}

			fx.blendFlag = chif::enums::BLENDMODE_ALPHA;
			if (colorspace != ColorSpace::SRGB)
			{
				fx.enableLinearOutputMapping(hdr_scaling);
			}

			chif::image::Draw(&tex, fx, cmd);
		}

		RenderPath2D::Compose(cmd);
	}

}
