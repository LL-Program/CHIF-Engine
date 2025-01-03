#pragma once
#include "chifRenderPath2D.h"
#include "chifColor.h"
#include "chifJobSystem.h"
#include "chifVector.h"

#include <functional>

namespace chif
{

	class Application;

	class LoadingScreen :
		public RenderPath2D
	{
	protected:
		chif::jobsystem::context ctx;
		chif::vector<std::function<void(chif::jobsystem::JobArgs)>> tasks;
		std::function<void()> finish;
		uint32_t launchedTasks = 0;
	public:
		chif::Resource backgroundTexture;

		enum class BackgroundMode
		{
			Fill,	// fill the whole screen, will cut off parts of the image if aspects don't match
			Fit,	// fit the image completely inside the screen, will result in black bars on screen if aspects don't match
			Stretch	// fill the whole screen, and stretch the image if needed
		} background_mode = BackgroundMode::Fill;

		//Add a loading task which should be executed
		void addLoadingFunction(std::function<void(chif::jobsystem::JobArgs)> loadingFunction);
		//Helper for loading a whole renderable component
		void addLoadingComponent(RenderPath* component, Application* main, float fadeSeconds = 0, chif::Color fadeColor = chif::Color(0, 0, 0, 255));
		//Set a function that should be called when the loading finishes
		void onFinished(std::function<void()> finishFunction);
		//See if the loading is currently running
		bool isActive() const;
		// See if there are any loading tasks that are still not finished
		bool isFinished() const;
		// Returns the percentage of loading tasks that are finished (0% - 100%)
		int getProgress() const;

		//Start Executing the tasks and mark the loading as active
		void Start() override;

		void Compose(chif::graphics::CommandList cmd) const override;
	};

}
