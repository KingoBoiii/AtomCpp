#pragma once

#include "KeyCodes.h"
#include <map>

namespace Atom
{

	struct KeyData
	{
		KeyCode Key;
		KeyState State = KeyState::None;
		KeyState OldState = KeyState::None;
	};

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyHeld(KeyCode keycode);
		static bool IsKeyDown(KeyCode keycode);
		static bool IsKeyReleased(KeyCode keycode);

		// Internal use only...
		static void TransitionPressedKeys();
		static void UpdateKeyState(KeyCode key, KeyState newState);
		static void ClearReleasedKeys();
	private:
		inline static std::map<KeyCode, KeyData> s_KeyData;
	};

}
