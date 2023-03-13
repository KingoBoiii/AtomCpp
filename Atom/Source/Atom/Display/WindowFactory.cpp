#include "ATPCH.h"
#include "WindowFactory.h"

#include "GLFW/GlfwWindow.h"
#include "Atom/Platform/Win32/Win32Window.h"

namespace Atom
{

	Window* WindowFactory::Create(const WindowOptions& windowOptions)
	{
#ifdef AT_PLATFORM_WINDOWS
		return new Win32Window(windowOptions);
#else
		return new GlfwWindow(windowOptions);
#endif
	}

}