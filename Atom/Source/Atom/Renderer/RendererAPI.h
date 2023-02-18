#pragma once
#include "Atom/Core/Base.h"

namespace Atom
{

	enum class RendererAPI
	{
		Unknown = BIT(0),
		OpenGL = BIT(2),
		DirectX11 = BIT(1)
	};

}
