#pragma once
#include "Core/Application.h"

#ifdef AT_PLATFORM_WINDOWS 

extern Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Atom::Application* app = Atom::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#endif
