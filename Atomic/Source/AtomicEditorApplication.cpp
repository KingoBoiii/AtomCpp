#include <Atom.h>
#include <Atom/EntryPoint.h>

#include "EditorLayer.h"

namespace Atom
{

	class AtomEditorApplication : public Atom::Application
	{
	public:
		AtomEditorApplication(const Atom::ApplicationOptions& applicationOptions, std::string_view projectPath)
			: Atom::Application(applicationOptions), m_ProjectPath(projectPath)
		{
			PushLayer(new EditorLayer(m_ProjectPath));
		}
	private:
		std::string m_ProjectPath;
	};

}

Atom::Application* Atom::CreateApplication(Atom::ApplicationCommandLineArgs args)
{
	std::string_view projectPath = "SandboxProject/Sandbox.atpr";
	if(args.Count > 1)
	{
		projectPath = args.Args[1];
	}

	Atom::ApplicationOptions applicationOptions{};
	applicationOptions.Title = "Atom";
	applicationOptions.Width = 1920;
	applicationOptions.Height = 1080;
	applicationOptions.Fullscreen = false;
	applicationOptions.StartMaximized = true;
	applicationOptions.VSync = true;
	applicationOptions.CommandLineArgs = args;
	
	applicationOptions.ScriptConfig.CoreAssemblyPath = "Resources/Scripts/Atom.Core.dll";

	return new Atom::AtomEditorApplication(applicationOptions, projectPath);
}
