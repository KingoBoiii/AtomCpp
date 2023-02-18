#include "ATPCH.h"
#include "GlfwWindow.h"

#include "Atom/Core/Assertion.h"
#include "Atom/Renderer/RendererContext.h"
#include "Atom/Renderer/SwapChain.h"

#include "Atom/Events/KeyEvent.h"
#include "Atom/Events/MouseEvent.h"
#include "Atom/Events/WindowEvent.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Atom
{

	namespace Utils
	{

		GLFWwindow* CreateGlfwWindow(const WindowOptions& windowOptions)
		{
			if(windowOptions.Fullscreen)
			{
				GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
				return glfwCreateWindow(mode->width, mode->height, windowOptions.Title.c_str(), primaryMonitor, nullptr);
			}

			return glfwCreateWindow(windowOptions.Width, windowOptions.Height, windowOptions.Title.c_str(), nullptr, nullptr);
		}

	}

	static void GLFWErrorCallback(int error, const char* description)
	{
		AT_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GlfwWindow::GlfwWindow(const WindowOptions& windowOptions)
		: Window(windowOptions)
	{
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(m_WindowHandle);

		glfwTerminate();
	}

	void GlfwWindow::Initialize()
	{
		int success = glfwInit();
		AT_CORE_ASSERT(success, "Failed to initialize GLFW!");

		glfwSetErrorCallback(GLFWErrorCallback);

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

		m_WindowHandle = Utils::CreateGlfwWindow(m_WindowOptions);
		AT_CORE_ASSERT(m_WindowHandle, "Failed to create GLFW window");

		m_RendererContext = RendererContext::Create(this);
		m_RendererContext->Initialize();

		m_SwapChain = SwapChain::Create(this);
		m_SwapChain->Initialize();

		//glfwMakeContextCurrent(m_WindowHandle);

		glfwSetWindowUserPointer(m_WindowHandle, &m_WindowData);

		SetupCallbacks();
	}

	void GlfwWindow::Update() const
	{
		glfwPollEvents();
	}

	void GlfwWindow::Present() const
	{
#if 0
		m_SwapChain->Present();
#endif
	}

	void* GlfwWindow::GetNativeWindowHandle() const
	{
		return static_cast<void*>(glfwGetWin32Window(m_WindowHandle));
	}

	void GlfwWindow::SetupCallbacks()
	{
		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window)
		{
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.EventCallback(e);
		});

		glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			data.EventCallback(e);
		});

		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch(action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.EventCallback(e);
				}
				break;
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.EventCallback(e);
				}
				break;
			}
		});

		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch(action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallback(e);
				}
				break;
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
				}
				break;
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.EventCallback(e);
				}
				break;
			}
		});
	}

}