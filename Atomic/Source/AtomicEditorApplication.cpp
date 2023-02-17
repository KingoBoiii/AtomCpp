#include <Atom.h>
#include <Atom/EntryPoint.h>

#include "EditorLayer.h"

namespace Atomic
{

	class AtomicEditorApplication : public Atom::Application
	{
	public:
		AtomicEditorApplication(const Atom::ApplicationOptions& applicationOptions)
			: Atom::Application(applicationOptions)
		{
			PushLayer(new EditorLayer());
		}

		virtual ~AtomicEditorApplication()
		{
		}
	};

}

Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args)
{
	Atom::ApplicationOptions applicationOptions{};
	applicationOptions.Title = "Atomic";
	applicationOptions.Width = 1366;
	applicationOptions.Height = 768;
	applicationOptions.Fullscreen = false;
	applicationOptions.VSync = true;
	applicationOptions.CommandLineArgs = args;

	return new Atomic::AtomicEditorApplication(applicationOptions);
}
