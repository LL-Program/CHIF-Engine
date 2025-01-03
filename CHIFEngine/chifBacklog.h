#pragma once
#include "CommonInclude.h"
#include "chifGraphicsDevice.h"
#include "chifCanvas.h"
#include "chifColor.h"

#include <string>

namespace chif::backlog
{
	// Do not modify the order, as this is exposed to LUA scripts as int!
	enum class LogLevel
	{
		None,
		Default,
		Warning,
		Error,
	};

	void Toggle();
	void Scroll(int direction);
	void Update(const chif::Canvas& canvas, float dt = 1.0f / 60.0f);
	void Draw(
		const chif::Canvas& canvas,
		chif::graphics::CommandList cmd,
		chif::graphics::ColorSpace colorspace = chif::graphics::ColorSpace::SRGB
	);
	void DrawOutputText(
		const chif::Canvas& canvas,
		chif::graphics::CommandList cmd,
		chif::graphics::ColorSpace colorspace = chif::graphics::ColorSpace::SRGB
	);

	std::string getText();
	void clear();
	void post(const char* input, LogLevel level = LogLevel::Default);
	void post(const std::string& input, LogLevel level = LogLevel::Default);

	void historyPrev();
	void historyNext();

	bool isActive();

	void setBackground(chif::graphics::Texture* texture);
	void setFontSize(int value);
	void setFontRowspacing(float value);
	void setFontColor(chif::Color color);

	void Lock();
	void Unlock();

	void BlockLuaExecution();
	void UnblockLuaExecution();

	void SetLogLevel(LogLevel newLevel);

	LogLevel GetUnseenLogLevelMax();


	// These are no longer used, but kept here to not break user code:
	inline void input(const char input) {}
	inline void acceptInput() {}
	inline void deletefromInput() {}
};
