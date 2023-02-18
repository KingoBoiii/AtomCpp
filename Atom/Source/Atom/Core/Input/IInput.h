#pragma once
#include "KeyCodes.h"

namespace Atom
{

	interface IInput
	{
		virtual bool IsKeyDown(KeyCode keycode) = 0;
	};

}
