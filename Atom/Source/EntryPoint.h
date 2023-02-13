#pragma once
#include "Core/Base.h"
#include "Core/Application.h"

#ifdef AT_PLATFORM_WINDOWS 

extern Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Atom::InitializeEngine();

	Atom::Application* app = Atom::CreateApplication({ argc, argv });
	app->Run();
	delete app;

	Atom::ShutdownEngine();
}

#endif
