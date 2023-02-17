#include "ATPCH.h"
#include "Atom/Core/Input.h"
#include "GlfwWindow.h"

#include "Atom/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Atom
{

	bool Input::IsKeyPressed(KeyCode key)
	{
		return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Pressed;
	}

	bool Input::IsKeyHeld(KeyCode key)
	{
		return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Held;
	}

	bool Input::IsKeyDown(KeyCode keycode)
	{
		auto window = static_cast<GlfwWindow*>(Application::Get().GetWindow());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window->GetNativeWindowHandle()), static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyReleased(KeyCode key)
	{
		return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Released;
	}

	void Input::TransitionPressedKeys()
	{
		for(const auto& [key, keyData] : s_KeyData)
		{
			if(keyData.State == KeyState::Pressed)
			{
				UpdateKeyState(key, KeyState::Held);
			}
		}
	}

	void Input::UpdateKeyState(KeyCode key, KeyState newState)
	{
		auto& keyData = s_KeyData[key];
		keyData.Key = key;
		keyData.OldState = keyData.State;
		keyData.State = newState;
	}

	void Input::ClearReleasedKeys()
	{
		for(const auto& [key, keyData] : s_KeyData)
		{
			if(keyData.State == KeyState::Released)
			{
				UpdateKeyState(key, KeyState::None);
			}
		}
	}

}

}