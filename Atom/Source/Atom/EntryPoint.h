#pragma once
#include "Core/Base.h"
#include "Core/Application.h"

#ifdef AT_PLATFORM_WINDOWS 

extern Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args);
bool g_ApplicationRunning = true;

int main(int argc, char** argv)
{
	while(g_ApplicationRunning)
	{
		Atom::InitializeEngine();

		Atom::Application* app = Atom::CreateApplication({ argc, argv });
		app->Run();
		delete app;

		Atom::ShutdownEngine();
	}
}

#endif
