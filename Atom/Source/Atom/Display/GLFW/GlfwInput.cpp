#include "ATPCH.h"
#include "Atom/Core/Input.h"
#include "GlfwWindow.h"

#include "Atom/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Atom
{
	
	bool Input::IsKeyDown(KeyCode keycode)
	{
		auto window = static_cast<GlfwWindow*>(Application::Get().GetWindow());

		auto state = glfwGetKey(window->m_WindowHandle, static_cast<int32_t>(keycode));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

}
