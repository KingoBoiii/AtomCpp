#include "ATPCH.h"
#include "GlfwWindow.h"

#include "Atom/Core/Assertion.h"
#include "Atom/Graphics/RendererContext.h"
#include "Atom/Graphics/SwapChain.h"

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

	GlfwWindow::GlfwWindow(const WindowOptions& windowOptions)
		: Window(windowOptions)
	{
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(m_WindowHandle);
	}

	void GlfwWindow::Initialize()
	{
		int success = glfwInit();
		AT_CORE_ASSERT(success, "Failed to initialize GLFW!");

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

		m_WindowHandle = Utils::CreateGlfwWindow(m_WindowOptions);
		AT_CORE_ASSERT(m_WindowHandle, "Failed to create GLFW window");

		m_RendererContext = RendererContextFactory::Create(this);
		m_RendererContext->Initialize();

		SwapChainOptions swapChainOptions{ };
		swapChainOptions.Width = m_WindowOptions.Width;
		swapChainOptions.Height = m_WindowOptions.Height;
		swapChainOptions.Window = this;
		m_SwapChain = SwapChainFactory::Create(swapChainOptions);
		m_SwapChain->Initialize();
		
		glfwMakeContextCurrent(m_WindowHandle);

		glfwSetWindowUserPointer(m_WindowHandle, &m_WindowData);

		SetupCallbacks();
	}

	void GlfwWindow::Update() const
	{
		glfwPollEvents();
	}

	void GlfwWindow::Present() const
	{
		m_SwapChain->Present();
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
	}

}