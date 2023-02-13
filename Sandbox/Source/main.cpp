#include <Atom.h>
#include <Atom/EntryPoint.h>

#include <iostream>

class SandboxApplication : public Atom::Application
{
public:
	SandboxApplication(const Atom::ApplicationOptions& applicationOptions)
		: Atom::Application(applicationOptions)
	{
		std::cout << "SandboxApplication::SandboxApplication()" << std::endl;
	}

	virtual ~SandboxApplication()
	{
		std::cout << "SandboxApplication::~SandboxApplication()" << std::endl;
	}
};

Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args)
{
	Atom::ApplicationOptions applicationOptions{};
	applicationOptions.CommandLineArgs = args;

	applicationOptions.Title = "Atom Engine - Sandbox";
	applicationOptions.Width = 1280;
	applicationOptions.Height = 720;
	applicationOptions.Fullscreen = false;
	applicationOptions.VSync = true;

	return new SandboxApplication(applicationOptions);
}