#include "ATPCH.h"
#include "Win32Input.h"

#include <Windows.h>
#include <Windowsx.h>

namespace Atom
{

	bool Win32Input::IsKeyDown(KeyCode keycode)
	{
		switch(keycode)
		{
			case Key::LeftControl:	keycode = VK_LCONTROL;	break;
			case Key::LeftAlt:		keycode = VK_LMENU;		break;
			case Key::LeftShift:	keycode = VK_LSHIFT;	break;
			default: break;
		}

		return GetKeyState(keycode) & 0x8000; // &(1 << 16);
	}

	bool Win32Input::IsMouseButtonPressed(MouseButtonCode button)
	{
		switch(button)
		{
			case MouseButton::Left:		button = VK_LBUTTON;	break;
			case MouseButton::Right:	button = VK_RBUTTON;	break;
			case MouseButton::Middle:	button = VK_MBUTTON;	break;
			default: break;
		}

		return GetKeyState(button) & 0x8000;
	}

	glm::vec2 Win32Input::GetMousePosition()
	{
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);
		return { static_cast<float>(cursorPosition.x), static_cast<float>(cursorPosition.y) };
	}

}
