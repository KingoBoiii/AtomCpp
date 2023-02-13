#pragma once
#include "Atom/Core/Base.h"
#include "Window.h"
#include "WindowOptions.h"

namespace Atom
{

	class ATOM_API WindowFactory
	{
	public:
		static Window* Create(const WindowOptions& windowOptions);
	};

}
