#pragma once
#include "Base.h"
#include "ApplicationOptions.h"
#include "ApplicationCommandLineArgs.h"

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
	private:
		ApplicationOptions m_ApplicationOptions;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);

}
