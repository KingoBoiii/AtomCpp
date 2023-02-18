#include "ATPCH.h"
#include "Win32Input.h"

#include <Windows.h>
#include <Windowsx.h>

namespace Atom
{

	bool Win32Input::IsKeyDown(KeyCode keycode)
	{
		if(keycode == Key::LeftControl)
		{
			keycode = 0xA2;
		}
		return GetKeyState(keycode) & 0x8000; // &(1 << 16);
	}

}
