#include "ATPCH.h"
#include "WindowFactory.h"

#include "GLFW/GlfwWindow.h"

namespace Atom
{

	Window* WindowFactory::Create(const WindowOptions& windowOptions)
	{
		return new GlfwWindow(windowOptions);
	}

}