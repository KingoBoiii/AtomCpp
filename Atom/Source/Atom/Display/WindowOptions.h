#pragma once
#include "Atom/Core/Base.h"

#include <string>

namespace Atom
{

	struct ATOM_API WindowOptions
	{
		std::string Title = "Atom Engine";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Fullscreen = false;
		bool VSync = true;
	};

}
