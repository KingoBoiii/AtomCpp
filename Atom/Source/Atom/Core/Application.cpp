#include "ATPCH.h"
#include "Application.h"

#include "Layer.h"
#include "Timer.h"

#include "Atom/Display/WindowFactory.h"
#include "Atom/Renderer/Renderer.h"
#include "Atom/Scripting/ScriptEngine.h"
#include "Atom/Physics/2D/Physics2D.h"
#include "Input/Input.h"

extern bool g_ApplicationRunning;

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

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationOptions& applicationOptions)
		: m_ApplicationOptions(applicationOptions)
	{
		AT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if(!m_ApplicationOptions.WorkingDirectory.empty())
		{
			std::filesystem::current_path(m_ApplicationOptions.WorkingDirectory);
		}

		m_Window = Utils::CreateWinddow(m_ApplicationOptions);
		m_Window->SetEventCallback(AT_BIND_EVENT_FN(Application::OnEvent));
		m_Window->Initialize();
		if(m_ApplicationOptions.StartMaximized)
		{
			m_Window->Maximize();
		}
		else
		{
			m_Window->CenterWindow();
		}

		Renderer::Initialize(m_Window);
		ScriptEngine::Initialize();
		Input::Initialize();
		Physics2D::Initialize();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		for(Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
			delete layer;
		}

		Physics2D::Shutdown();
		Input::Shutdown();
		ScriptEngine::Shutdown();
		Renderer::Shutdown();

		delete m_Window;
		s_Instance = nullptr;
	}

	void Application::Run()
	{
		Timer timer;

		while(m_IsRunning)
		{
			m_Window->Update();

			float time = timer.Elapsed();
			m_DeltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			Renderer::BeginFrame();
			for(Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(m_DeltaTime);
			}

			m_ImGuiLayer->Begin();
			{
				for(Layer* layer : m_LayerStack)
				{
					layer->OnGUI();
				}
			}
			m_ImGuiLayer->End();
			Renderer::EndFrame();

			m_Window->Present();
		}
	}

	void Application::Restart()
	{
		m_IsRunning = false;
	}

	void Application::Close()
	{
		g_ApplicationRunning = false;
		m_IsRunning = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(AT_BIND_EVENT_FN(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(AT_BIND_EVENT_FN(Application::OnWindowResizeEvent));

		for(auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if(e.Handled)
				break;
		}
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		if(e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			return false;
		}

		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

		return true;
	}

}