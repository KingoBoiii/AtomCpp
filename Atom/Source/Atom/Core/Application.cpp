#include "ATPCH.h"
#include "Application.h"

#include "Atom/Display/WindowFactory.h"
#include "Atom/Graphics/Renderer.h"

namespace Atom
{

	namespace Utils
	{

		Window* CreateWinddow(const ApplicationOptions& applicationOptions)
		{
			WindowOptions windowOptions{ };
			windowOptions.Title = applicationOptions.Title;
			windowOptions.Width = applicationOptions.Width;
			windowOptions.Height = applicationOptions.Height;
			windowOptions.Fullscreen = applicationOptions.Fullscreen;
			windowOptions.VSync = applicationOptions.VSync;

			return WindowFactory::Create(windowOptions);
		}

	}

	Application::Application(const ApplicationOptions& applicationOptions)
		: m_ApplicationOptions(applicationOptions)
	{
		m_Window = Utils::CreateWinddow(m_ApplicationOptions);
		m_Window->SetEventCallback(AT_BIND_EVENT_FN(Application::OnEvent));
		m_Window->Initialize();

		RendererOptions rendererOptions{ };
		rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
		rendererOptions.SwapChain = m_Window->GetSwapChain();
		m_Renderer = RendererFactory::Create(rendererOptions);
		m_Renderer->Initialize();
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::Run()
	{
		while(m_IsRunning)
		{
			m_Window->Update();

			m_Renderer->Clear();

			m_Window->Present();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(AT_BIND_EVENT_FN(Application::OnWindowCloseEvent));
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

}