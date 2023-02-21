#pragma once
#include "KeyCodes.h"
#include "MouseButtons.h"

namespace Atom
{

	class Input
	{
	public:
		static void Initialize();
		static void Shutdown();

		static bool IsKeyDown(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseButtonCode button);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
