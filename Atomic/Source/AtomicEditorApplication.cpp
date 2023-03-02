#include <Atom.h>
#include <Atom/EntryPoint.h>

#include "EditorLayer.h"

namespace Atom
{

#define ATOM_ENVIRONMENT_KEY "ATOM_DIR"

	class AtomEditorApplication : public Atom::Application
	{
	public:
		AtomEditorApplication(const Atom::ApplicationOptions& applicationOptions, std::string_view projectPath)
			: Atom::Application(applicationOptions), m_ProjectPath(projectPath)
		{
			UpdateEnvorinmentVariable(); // Update the ATOM_DIR environment variable every time we launch

			PushLayer(new EditorLayer(m_ProjectPath));
		}
	private:
		void UpdateEnvorinmentVariable()
		{
			std::filesystem::path workingDirectory = std::filesystem::current_path();

			if(workingDirectory.stem().string() == "Atomic")
			{
				workingDirectory = workingDirectory.parent_path();
			}

			Atom::Regex::SetEnvironmentVariable(ATOM_ENVIRONMENT_KEY, workingDirectory.string());
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
	applicationOptions.Title = "Atomic";
	applicationOptions.Width = 1920;
	applicationOptions.Height = 1080;
	applicationOptions.Fullscreen = false;
	applicationOptions.StartMaximized = true;
	applicationOptions.VSync = true;
	applicationOptions.CommandLineArgs = args;

	applicationOptions.ScriptConfig.CoreAssemblyPath = "Resources/Scripts/Atom.Core.dll";
	applicationOptions.ScriptConfig.EnableDebugging = true;

	return new Atom::AtomEditorApplication(applicationOptions, projectPath);
}
