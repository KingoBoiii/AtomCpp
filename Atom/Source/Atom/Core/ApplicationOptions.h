#pragma once
#include "Base.h"
#include "ApplicationCommandLineArgs.h"

#include "Atom/Scripting/ScriptEngine.h"

namespace Atom
{
	
	struct ATOM_API ApplicationOptions
	{
		std::filesystem::path WorkingDirectory;

		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool Fullscreen;
		bool VSync;
		bool StartMaximized;

		ScriptEngineConfig ScriptConfig;

		ApplicationCommandLineArgs CommandLineArgs;
	};

}
