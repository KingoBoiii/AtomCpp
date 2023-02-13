#include <Atom.h>
#include <EntryPoint.h>

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

	return new SandboxApplication(applicationOptions);
}