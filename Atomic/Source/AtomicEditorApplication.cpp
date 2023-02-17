#include <Atom.h>
#include <Atom/EntryPoint.h>

namespace Atomic
{

	class AtomicEditorApplication : public Atom::Application
	{
	public:
		AtomicEditorApplication(const Atom::ApplicationOptions& applicationOptions)
			: Atom::Application(applicationOptions)
		{
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
	applicationOptions.CommandLineArgs = args;

	return new Atomic::AtomicEditorApplication(applicationOptions);
}
