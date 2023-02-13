#pragma once
#include "Base.h"
#include "ApplicationOptions.h"
#include "ApplicationCommandLineArgs.h"

#include "Atom/Display/Window.h"
#include "Atom/Events/Event.h"
#include "Atom/Events/WindowEvent.h"

namespace Atom
{

	class ATOM_API Application
	{
	public:
		Application() = delete;
		Application(const Application&) = delete;
		Application(const ApplicationOptions& applicationOptions);
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowCloseEvent(WindowCloseEvent& e);
	private:
		ApplicationOptions m_ApplicationOptions;
		Window* m_Window = nullptr;
		bool m_IsRunning = true;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);

}
