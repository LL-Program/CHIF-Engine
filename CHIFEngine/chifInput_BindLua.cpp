#include "chifInput_BindLua.h"
#include "chifMath_BindLua.h"

namespace chif::lua
{

	Luna<Input_BindLua>::FunctionType Input_BindLua::methods[] = {
		lunamethod(Input_BindLua, Down),
		lunamethod(Input_BindLua, Press),
		lunamethod(Input_BindLua, Release),
		lunamethod(Input_BindLua, Hold),
		lunamethod(Input_BindLua, GetPointer),
		lunamethod(Input_BindLua, SetPointer),
		lunamethod(Input_BindLua, GetPointerDelta),
		lunamethod(Input_BindLua, HidePointer),
		lunamethod(Input_BindLua, GetAnalog),
		lunamethod(Input_BindLua, GetTouches),
		lunamethod(Input_BindLua, SetControllerFeedback),
		lunamethod(Input_BindLua, WhatIsPressed),
		lunamethod(Input_BindLua, IsGamepadButton),
		lunamethod(Input_BindLua, ButtonToString),
		lunamethod(Input_BindLua, StringToButton),
		lunamethod(Input_BindLua, SetCursor),
		lunamethod(Input_BindLua, SetCursorFromFile),
		lunamethod(Input_BindLua, ResetCursor),
		{ NULL, NULL }
	};
	Luna<Input_BindLua>::PropertyType Input_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int Input_BindLua::Down(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::BUTTON button = (chif::input::BUTTON)chif::lua::SGetInt(L, 1);
			int playerindex = 0;
			if (argc > 1)
			{
				playerindex = chif::lua::SGetInt(L, 2);
			}
			chif::lua::SSetBool(L, chif::input::Down(button, playerindex));
			return 1;
		}
		else
			chif::lua::SError(L, "Down(int code, opt int playerindex = 0) not enough arguments!");
		return 0;
	}
	int Input_BindLua::Press(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::BUTTON code = (chif::input::BUTTON)chif::lua::SGetInt(L, 1);
			int playerindex = 0;
			if (argc > 1)
			{
				playerindex = chif::lua::SGetInt(L, 2);
			}
			chif::lua::SSetBool(L, chif::input::Press(code, playerindex));
			return 1;
		}
		else
			chif::lua::SError(L, "Press(int code, opt int playerindex = 0) not enough arguments!");
		return 0;
	}
	int Input_BindLua::Release(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::BUTTON code = (chif::input::BUTTON)chif::lua::SGetInt(L, 1);
			int playerindex = 0;
			if (argc > 1)
			{
				playerindex = chif::lua::SGetInt(L, 2);
			}
			chif::lua::SSetBool(L, chif::input::Release(code, playerindex));
			return 1;
		}
		else
			chif::lua::SError(L, "Release(int code, opt int playerindex = 0) not enough arguments!");
		return 0;
	}
	int Input_BindLua::Hold(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::BUTTON button = (chif::input::BUTTON)chif::lua::SGetInt(L, 1);
			uint32_t duration = 30;
			if (argc > 1)
			{
				duration = chif::lua::SGetInt(L, 2);
			}
			bool continuous = false;
			if (argc > 2)
			{
				continuous = chif::lua::SGetBool(L, 3);
			}
			int playerindex = 0;
			if (argc > 3)
			{
				playerindex = chif::lua::SGetInt(L, 4);
			}
			chif::lua::SSetBool(L, chif::input::Hold(button, duration, continuous, playerindex));
			return 1;
		}
		else
			chif::lua::SError(L, "Hold(int code, opt int duration = 30, opt boolean continuous = false, opt int playerindex = 0) not enough arguments!");
		return 0;
	}
	int Input_BindLua::GetPointer(lua_State* L)
	{
		XMFLOAT4 P = chif::input::GetPointer();
		Luna<Vector_BindLua>::push(L, XMLoadFloat4(&P));
		return 1;
	}
	int Input_BindLua::SetPointer(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
			if (vec != nullptr)
			{
				chif::input::SetPointer(vec->data);
			}
			else
				chif::lua::SError(L, "SetPointer(Vector props) argument is not a Vector!");
		}
		else
			chif::lua::SError(L, "SetPointer(Vector props) not enough arguments!");
		return 0;
	}
	int Input_BindLua::GetPointerDelta(lua_State* L)
	{
		Luna<Vector_BindLua>::push(L, XMLoadFloat2(&chif::input::GetMouseState().delta_position));
		return 1;
	}
	int Input_BindLua::HidePointer(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::HidePointer(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "HidePointer(bool value) not enough arguments!");
		return 0;
	}
	int Input_BindLua::GetAnalog(lua_State* L)
	{
		XMFLOAT4 result = XMFLOAT4(0, 0, 0, 0);

		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::GAMEPAD_ANALOG type = (chif::input::GAMEPAD_ANALOG)chif::lua::SGetInt(L, 1);
			int playerindex = 0;
			if (argc > 1)
			{
				playerindex = chif::lua::SGetInt(L, 2);
			}
			result = chif::input::GetAnalog(type, playerindex);
		}
		else
			chif::lua::SError(L, "GetAnalog(int type, opt int playerindex = 0) not enough arguments!");

		Luna<Vector_BindLua>::push(L, XMLoadFloat4(&result));
		return 1;
	}
	int Input_BindLua::GetTouches(lua_State* L)
	{
		auto& touches = chif::input::GetTouches();
		for (auto& touch : touches)
		{
			Luna<Touch_BindLua>::push(L, touch);
		}
		return (int)touches.size();
	}
	int Input_BindLua::SetControllerFeedback(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			ControllerFeedback_BindLua* fb = Luna<ControllerFeedback_BindLua>::lightcheck(L, 1); // Ferdinand Braun
			if (fb != nullptr)
			{
				int playerindex = 0;
				if (argc > 1)
				{
					playerindex = chif::lua::SGetInt(L, 2);
				}
				chif::input::SetControllerFeedback(fb->feedback, playerindex);
			}
			else
			{
				chif::lua::SError(L, "SetControllerFeedback(ControllerFeedback feedback, opt int playerindex = 0) first argument is not a ControllerFeedback!");
			}
		}
		else
			chif::lua::SError(L, "SetControllerFeedback(ControllerFeedback feedback, opt int playerindex = 0) not enough arguments!");
		return 0;
	}
	int Input_BindLua::WhatIsPressed(lua_State* L)
	{
		int playerindex = 0;
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			playerindex = chif::lua::SGetInt(L, 1);
		}
		chif::lua::SSetInt(L, (int)chif::input::WhatIsPressed(playerindex));
		return 1;
	}
	int Input_BindLua::IsGamepadButton(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc < 1)
		{
			chif::lua::SError(L, "IsGamepadButton(int button) not enough arguments!");
			return 0;
		}
		chif::input::BUTTON button = (chif::input::BUTTON)chif::lua::SGetInt(L, 1);
		chif::lua::SSetBool(L, chif::input::IsGamepadButton(button));
		return 1;
	}
	int Input_BindLua::ButtonToString(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::BUTTON button = (chif::input::BUTTON)chif::lua::SGetInt(L, 1);
			chif::input::CONTROLLER_PREFERENCE preference = chif::input::CONTROLLER_PREFERENCE_GENERIC;
			if (argc > 1)
			{
				preference = (chif::input::CONTROLLER_PREFERENCE)chif::lua::SGetInt(L, 2);
			}
			chif::lua::SSetString(L, chif::input::ButtonToString(button, preference));
			return 1;
		}
		chif::lua::SError(L, "ButtonToString(int button, opt int preference = CONTROLLER_PREFERENCE_GENERIC): not enough parameters!");
		return 0;
	}
	int Input_BindLua::StringToButton(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			const char* str = chif::lua::SGetString(L, 1);
			chif::lua::SSetInt(L, (int)chif::input::StringToButton(str));
			return 1;
		}
		chif::lua::SError(L, "StringToButton(string str): not enough parameters!");
		return 0;
	}
	int Input_BindLua::SetCursor(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::CURSOR cursor = (chif::input::CURSOR)chif::lua::SGetInt(L, 1);
			chif::input::SetCursor(cursor);
			return 0;
		}
		chif::lua::SError(L, "SetCursor(int cursor) not enough arguments!");
		return 0;
	}
	int Input_BindLua::SetCursorFromFile(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			chif::input::CURSOR cursor = (chif::input::CURSOR)chif::lua::SGetInt(L, 1);
			const char* filename = chif::lua::SGetString(L, 2);
			chif::input::SetCursorFromFile(cursor, filename);
			return 0;
		}
		chif::lua::SError(L, "SetCursorFromFile(int cursor, string filename) not enough arguments!");
		return 0;
	}
	int Input_BindLua::ResetCursor(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::input::CURSOR cursor = (chif::input::CURSOR)chif::lua::SGetInt(L, 1);
			chif::input::ResetCursor(cursor);
			return 0;
		}
		chif::lua::SError(L, "ResetCursor(int cursor) not enough arguments!");
		return 0;
	}

	void Input_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<Input_BindLua>::Register(chif::lua::GetLuaState());

			chif::lua::RunText(R"(
input = Input()

BUTTON_NONE = 0

GAMEPAD_BUTTON_UP			= 257
GAMEPAD_BUTTON_LEFT			= 258
GAMEPAD_BUTTON_DOWN			= 259
GAMEPAD_BUTTON_RIGHT		= 260
GAMEPAD_BUTTON_1			= 261
GAMEPAD_BUTTON_2			= 262
GAMEPAD_BUTTON_3			= 263
GAMEPAD_BUTTON_4			= 264
GAMEPAD_BUTTON_5			= 265
GAMEPAD_BUTTON_6			= 266
GAMEPAD_BUTTON_7			= 267
GAMEPAD_BUTTON_8			= 268
GAMEPAD_BUTTON_9			= 269
GAMEPAD_BUTTON_10			= 270
GAMEPAD_BUTTON_11			= 271
GAMEPAD_BUTTON_12			= 272
GAMEPAD_BUTTON_13			= 273
GAMEPAD_BUTTON_14			= 274

GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_UP		= 275
GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_LEFT		= 276
GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_DOWN		= 277
GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_RIGHT		= 278
GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_UP		= 279
GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_LEFT		= 280
GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_DOWN		= 281
GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_RIGHT		= 282
GAMEPAD_ANALOG_TRIGGER_L_AS_BUTTON				= 283
GAMEPAD_ANALOG_TRIGGER_R_AS_BUTTON				= 284

GAMEPAD_BUTTON_XBOX_X = GAMEPAD_BUTTON_1
GAMEPAD_BUTTON_XBOX_A = GAMEPAD_BUTTON_2
GAMEPAD_BUTTON_XBOX_B = GAMEPAD_BUTTON_3
GAMEPAD_BUTTON_XBOX_Y = GAMEPAD_BUTTON_4
GAMEPAD_BUTTON_XBOX_L1 = GAMEPAD_BUTTON_5
GAMEPAD_BUTTON_XBOX_LT = GAMEPAD_ANALOG_TRIGGER_L_AS_BUTTON
GAMEPAD_BUTTON_XBOX_R1 = GAMEPAD_BUTTON_6
GAMEPAD_BUTTON_XBOX_RT = GAMEPAD_ANALOG_TRIGGER_R_AS_BUTTON
GAMEPAD_BUTTON_XBOX_L3 = GAMEPAD_BUTTON_7
GAMEPAD_BUTTON_XBOX_R3 = GAMEPAD_BUTTON_8
GAMEPAD_BUTTON_XBOX_BACK = GAMEPAD_BUTTON_9
GAMEPAD_BUTTON_XBOX_START = GAMEPAD_BUTTON_10

GAMEPAD_BUTTON_PLAYSTATION_SQUARE = GAMEPAD_BUTTON_1
GAMEPAD_BUTTON_PLAYSTATION_CROSS = GAMEPAD_BUTTON_2
GAMEPAD_BUTTON_PLAYSTATION_CIRCLE = GAMEPAD_BUTTON_3
GAMEPAD_BUTTON_PLAYSTATION_TRIANGLE = GAMEPAD_BUTTON_4
GAMEPAD_BUTTON_PLAYSTATION_L1 = GAMEPAD_BUTTON_5
GAMEPAD_BUTTON_PLAYSTATION_L2 = GAMEPAD_ANALOG_TRIGGER_L_AS_BUTTON
GAMEPAD_BUTTON_PLAYSTATION_R1 = GAMEPAD_BUTTON_6
GAMEPAD_BUTTON_PLAYSTATION_R2 = GAMEPAD_ANALOG_TRIGGER_R_AS_BUTTON
GAMEPAD_BUTTON_PLAYSTATION_L3 = GAMEPAD_BUTTON_7
GAMEPAD_BUTTON_PLAYSTATION_R3 = GAMEPAD_BUTTON_8
GAMEPAD_BUTTON_PLAYSTATION_SHARE = GAMEPAD_BUTTON_9
GAMEPAD_BUTTON_PLAYSTATION_OPTION = GAMEPAD_BUTTON_10
GAMEPAD_BUTTON_PLAYSTATION_SELECT = GAMEPAD_BUTTON_PLAYSTATION_SHARE
GAMEPAD_BUTTON_PLAYSTATION_START = GAMEPAD_BUTTON_PLAYSTATION_OPTION
			
GAMEPAD_ANALOG_THUMBSTICK_L	= 0
GAMEPAD_ANALOG_THUMBSTICK_R	= 1
GAMEPAD_ANALOG_TRIGGER_L	= 2
GAMEPAD_ANALOG_TRIGGER_R	= 3
			
TOUCHSTATE_PRESSED			= 0
TOUCHSTATE_RELEASED			= 1
TOUCHSTATE_MOVED			= 2

MOUSE_BUTTON_LEFT			= 513
MOUSE_BUTTON_RIGHT			= 514
MOUSE_BUTTON_MIDDLE			= 515

MOUSE_SCROLL_AS_BUTTON_UP	= 516
MOUSE_SCROLL_AS_BUTTON_DOWN	= 517

KEYBOARD_BUTTON_UP			= 518
KEYBOARD_BUTTON_DOWN		= 519
KEYBOARD_BUTTON_LEFT		= 520
KEYBOARD_BUTTON_RIGHT		= 521
KEYBOARD_BUTTON_SPACE		= 522
KEYBOARD_BUTTON_RSHIFT		= 523
KEYBOARD_BUTTON_LSHIFT		= 524
KEYBOARD_BUTTON_F1			= 525
KEYBOARD_BUTTON_F2			= 526
KEYBOARD_BUTTON_F3			= 527
KEYBOARD_BUTTON_F4			= 528
KEYBOARD_BUTTON_F5			= 529
KEYBOARD_BUTTON_F6			= 530
KEYBOARD_BUTTON_F7			= 531
KEYBOARD_BUTTON_F8			= 532
KEYBOARD_BUTTON_F9			= 533
KEYBOARD_BUTTON_F10			= 534
KEYBOARD_BUTTON_F11			= 535
KEYBOARD_BUTTON_F12			= 536
KEYBOARD_BUTTON_ENTER		= 537
KEYBOARD_BUTTON_ESCAPE		= 538
KEYBOARD_BUTTON_HOME		= 539
KEYBOARD_BUTTON_RCONTROL	= 540
KEYBOARD_BUTTON_LCONTROL	= 541
KEYBOARD_BUTTON_DELETE		= 542
KEYBOARD_BUTTON_BACK		= 543
KEYBOARD_BUTTON_PAGEDOWN	= 544
KEYBOARD_BUTTON_PAGEUP		= 545
KEYBOARD_BUTTON_NUMPAD0		= 546
KEYBOARD_BUTTON_NUMPAD1		= 547
KEYBOARD_BUTTON_NUMPAD2		= 548
KEYBOARD_BUTTON_NUMPAD3		= 549
KEYBOARD_BUTTON_NUMPAD4		= 550
KEYBOARD_BUTTON_NUMPAD5		= 551
KEYBOARD_BUTTON_NUMPAD6		= 552
KEYBOARD_BUTTON_NUMPAD7		= 553
KEYBOARD_BUTTON_NUMPAD8		= 554
KEYBOARD_BUTTON_NUMPAD9		= 555
KEYBOARD_BUTTON_MULTIPLY	= 556
KEYBOARD_BUTTON_ADD			= 557
KEYBOARD_BUTTON_SEPARATOR	= 558
KEYBOARD_BUTTON_SUBTRACT	= 559
KEYBOARD_BUTTON_DECIMAL		= 560
KEYBOARD_BUTTON_DIVIDE		= 561
KEYBOARD_BUTTON_TAB			= 562
KEYBOARD_BUTTON_TILDE		= 563
KEYBOARD_BUTTON_INSERT		= 564
KEYBOARD_BUTTON_ALT			= 565
KEYBOARD_BUTTON_ALTGR		= 566


CONTROLLER_PREFERENCE_GENERIC = 0
CONTROLLER_PREFERENCE_PLAYSTATION = 1
CONTROLLER_PREFERENCE_XBOX = 2

CURSOR_DEFAULT = 0
CURSOR_TEXTINPUT = 1
CURSOR_RESIZEALL = 2
CURSOR_RESIZE_NS = 3
CURSOR_RESIZE_EW = 4
CURSOR_RESIZE_NESW = 5
CURSOR_RESIZE_NWSE = 6
CURSOR_HAND = 7
CURSOR_NOTALLOWED = 8
CURSOR_CROSS = 9
)");

		}

		Touch_BindLua::Bind();
		ControllerFeedback_BindLua::Bind();
	}







	Luna<Touch_BindLua>::FunctionType Touch_BindLua::methods[] = {
		lunamethod(Touch_BindLua, GetState),
		lunamethod(Touch_BindLua, GetPos),
		{ NULL, NULL }
	};
	Luna<Touch_BindLua>::PropertyType Touch_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int Touch_BindLua::GetState(lua_State* L)
	{
		chif::lua::SSetInt(L, (int)touch.state);
		return 1;
	}
	int Touch_BindLua::GetPos(lua_State* L)
	{
		Luna<Vector_BindLua>::push(L, XMLoadFloat2(&touch.pos));
		return 1;
	}

	void Touch_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<Touch_BindLua>::Register(chif::lua::GetLuaState());
		}
	}







	Luna<ControllerFeedback_BindLua>::FunctionType ControllerFeedback_BindLua::methods[] = {
		lunamethod(ControllerFeedback_BindLua, SetVibrationRight),
		lunamethod(ControllerFeedback_BindLua, SetVibrationLeft),
		lunamethod(ControllerFeedback_BindLua, SetLEDColor),
		{ NULL, NULL }
	};
	Luna<ControllerFeedback_BindLua>::PropertyType ControllerFeedback_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int ControllerFeedback_BindLua::SetVibrationLeft(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			feedback.vibration_left = chif::lua::SGetFloat(L, 1);
		}
		else
		{
			chif::lua::SError(L, "SetVibrationLeft(float value) not enough arguments!");
		}
		return 0;
	}
	int ControllerFeedback_BindLua::SetVibrationRight(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			feedback.vibration_right = chif::lua::SGetFloat(L, 1);
		}
		else
		{
			chif::lua::SError(L, "SetVibrationRight(float value) not enough arguments!");
		}
		return 0;
	}
	int ControllerFeedback_BindLua::SetLEDColor(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 1);
			if (vec != nullptr)
			{
				feedback.led_color = chif::Color::fromFloat4(vec->data);
			}
			else
			{
				feedback.led_color.rgba = chif::lua::SGetInt(L, 1);
			}
		}
		else
		{
			chif::lua::SError(L, "SetLEDColor(int hexcolor) not enough arguments!");
		}
		return 0;
	}

	void ControllerFeedback_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<ControllerFeedback_BindLua>::Register(chif::lua::GetLuaState());
		}
	}

}
