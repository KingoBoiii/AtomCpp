#pragma once
#include "Base.h"
#include "ApplicationOptions.h"
#include "ApplicationCommandLineArgs.h"

#include "LayerStack.h"

#include "Atom/Display/Window.h"
#include "Atom/Events/Event.h"
#include "Atom/Events/WindowEvent.h"

#include "Atom/ImGui/ImGuiLayer.h"

namespace Atom
{

	class ATOM_API Renderer;
	
	class ATOM_API Application
	{
	public:
		Application() = delete;
		Application(const Application&) = delete;
		Application(const ApplicationOptions& applicationOptions);
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window* GetWindow() const { return m_Window; }
		static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowCloseEvent(WindowCloseEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		ApplicationOptions m_ApplicationOptions;
		Window* m_Window = nullptr;
		ImGuiLayer* m_ImGuiLayer = nullptr;

		LayerStack m_LayerStack;
		bool m_IsRunning = true;
		float m_DeltaTime = 0.0f;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);

}
