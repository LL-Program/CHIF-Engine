#pragma once
#include "chifLua.h"
#include "chifLuna.h"
#include "chifInput.h"

namespace chif::lua
{

	class Input_BindLua
	{
	public:
		inline static constexpr char className[] = "Input";
		static Luna<Input_BindLua>::FunctionType methods[];
		static Luna<Input_BindLua>::PropertyType properties[];

		Input_BindLua(lua_State* L) {}

		int Down(lua_State* L);
		int Press(lua_State* L);
		int Release(lua_State* L);
		int Hold(lua_State* L);
		int GetPointer(lua_State* L);
		int SetPointer(lua_State* L);
		int GetPointerDelta(lua_State* L);
		int HidePointer(lua_State* L);
		int GetAnalog(lua_State* L);
		int GetTouches(lua_State* L);
		int SetControllerFeedback(lua_State* L);
		int WhatIsPressed(lua_State* L);
		int IsGamepadButton(lua_State* L);
		int ButtonToString(lua_State* L);
		int StringToButton(lua_State* L);
		int SetCursor(lua_State* L);
		int SetCursorFromFile(lua_State* L);
		int ResetCursor(lua_State* L);

		static void Bind();
	};

	class Touch_BindLua
	{
	public:
		chif::input::Touch touch;
		inline static constexpr char className[] = "Touch";
		static Luna<Touch_BindLua>::FunctionType methods[];
		static Luna<Touch_BindLua>::PropertyType properties[];

		Touch_BindLua(lua_State* L) {}
		Touch_BindLua(const chif::input::Touch& touch) :touch(touch) {}

		int GetState(lua_State* L);
		int GetPos(lua_State* L);

		static void Bind();
	};

	class ControllerFeedback_BindLua
	{
	public:
		chif::input::ControllerFeedback feedback;
		inline static constexpr char className[] = "ControllerFeedback";
		static Luna<ControllerFeedback_BindLua>::FunctionType methods[];
		static Luna<ControllerFeedback_BindLua>::PropertyType properties[];

		ControllerFeedback_BindLua(lua_State* L) {}
		ControllerFeedback_BindLua(const chif::input::ControllerFeedback& feedback) :feedback(feedback) {}

		int SetVibrationLeft(lua_State* L);
		int SetVibrationRight(lua_State* L);
		int SetLEDColor(lua_State* L);

		static void Bind();
	};

}
