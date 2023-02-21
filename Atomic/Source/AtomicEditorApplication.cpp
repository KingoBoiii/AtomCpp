#include <Atom.h>
#include <Atom/EntryPoint.h>

#include "EditorLayer.h"

namespace Atom
{

	class AtomEditorApplication : public Atom::Application
	{
	public:
		AtomEditorApplication(const Atom::ApplicationOptions& applicationOptions)
			: Atom::Application(applicationOptions)
		{
			PushLayer(new EditorLayer());
		}
	};

}

Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args)
{
	Atom::ApplicationOptions applicationOptions{};
	applicationOptions.Title = "Atom";
	applicationOptions.Width = 1600;
	applicationOptions.Height = 900;
	applicationOptions.Fullscreen = false;
	applicationOptions.VSync = true;
	applicationOptions.CommandLineArgs = args;

	return new Atom::AtomEditorApplication(applicationOptions);
}
