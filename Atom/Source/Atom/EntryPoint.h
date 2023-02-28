#pragma once

#ifdef AT_PLATFORM_WINDOWS 

extern Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args);
bool g_ApplicationRunning = true;

namespace Atom
{

	int Main(int argc, char** argv)
	{
		while(g_ApplicationRunning)
		{
			Atom::InitializeEngine();

			Atom::Application* app = Atom::CreateApplication({ argc, argv });
			app->Run();
			delete app;

			Atom::ShutdownEngine();
		}

		return 0;
	}

}

#ifdef AT_RELEASE 

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int nCmdShow)
{
	return Atom::Main(__argc, __argv);
}

#else 

int main(int argc, char** argv)
{
	return Atom::Main(argc, argv);
}

#endif

#if 0
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

#endif
