#pragma once
#include "Atom/Core/Base.h"

namespace Atom
{

	enum class RendererAPI
	{
		Unknown = BIT(0),
		// Cross-platform
		OpenGL = BIT(1),
		// Cross-platform
		Vulkan = BIT(2),

		// Windows-only
		DirectX11 = BIT(3),
		// Windows-only
		DirectX12 = BIT(4),

		// MacOS-only
		Metal = BIT(5)
	};

}
