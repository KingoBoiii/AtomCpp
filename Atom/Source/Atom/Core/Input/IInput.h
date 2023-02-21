#pragma once
#include "KeyCodes.h"
#include "MouseButtons.h"

namespace Atom
{

	interface IInput
	{
		virtual bool IsKeyDown(KeyCode keycode) = 0;

		virtual bool IsMouseButtonPressed(MouseButtonCode button) = 0;

		virtual glm::vec2 GetMousePosition() = 0;
	};

}
