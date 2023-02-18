#pragma once

#include "KeyCodes.h"
#include "MouseButtons.h"
#include <map>

namespace Atom
{

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode keycode);
	};

}
