#pragma once
#include "KeyCodes.h"

namespace Atom
{

	class Input
	{
	public:
		static void Initialize();
		static void Shutdown();

		static bool IsKeyDown(KeyCode keycode);
	};

}
