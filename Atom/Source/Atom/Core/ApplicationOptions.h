#pragma once
#include "Base.h"
#include "ApplicationCommandLineArgs.h"

namespace Atom
{
	
	struct ATOM_API ApplicationOptions
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool Fullscreen;
		bool VSync;

		ApplicationCommandLineArgs CommandLineArgs;
	};

}
