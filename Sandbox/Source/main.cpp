#include <Atom.h>
#include <Atom/EntryPoint.h>

#include "Layers/SandboxLayer.h"
#include "Layers/Renderer2DLayer.h"
#include "Layers/SceneTestLayer.h"

class SandboxApplication : public Atom::Application
{
public:
	SandboxApplication(const Atom::ApplicationOptions& applicationOptions)
		: Atom::Application(applicationOptions)
	{
		//PushLayer(new SandboxLayer());
		//PushLayer(new Renderer2DLayer());
		PushLayer(new SceneTestLayer());
	}

	virtual ~SandboxApplication()
	{
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