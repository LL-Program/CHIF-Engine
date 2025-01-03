#pragma once
#include "CommonInclude.h"
#include "chifColor.h"
#include "chifTimer.h"
#include "chifMath.h"

#include <functional>

namespace chif
{
	class FadeManager
	{
	public:
		float opacity = 0;
		float timer = 0;
		float targetFadeTimeInSeconds = 1.0f;
		enum FADE_STATE
		{
			FADE_IN,	// no fade -> faded
			FADE_MID,	// completely faded
			FADE_OUT,	// faded -> no fade
			FADE_FINISHED,
		} state = FADE_FINISHED;
		chif::Color color = chif::Color(0, 0, 0, 255);
		std::function<void()> onFade = [] {};
		bool fadeEventTriggeredThisFrame = false;

		FadeManager()
		{
			Clear();
		}
		void Clear();
		void Start(float seconds, chif::Color color, std::function<void()> onFadeFunction)
		{
			targetFadeTimeInSeconds = seconds;
			this->color = color;
			timer = 0;
			if (IsFaded())
			{
				// If starting a new fade on mid-fadeout, it will start from faded and just transition out of mid
				state = FADE_MID;
			}
			else
			{
				state = FADE_IN;
			}
			onFade = onFadeFunction;
		}
		void Update(float dt);
		bool IsFaded() const
		{
			return fadeEventTriggeredThisFrame;
		}
		bool IsActive() const
		{
			return state != FADE_FINISHED;
		}
	};
}
