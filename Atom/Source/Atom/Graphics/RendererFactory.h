#pragma once

#include "Renderer.h"

namespace Atom
{
	
	class ATOM_API Renderer2D;

	class ATOM_API RendererFactory
	{
	public:
		static Renderer* CreateRenderer(const RendererOptions& rendererOptions);
		static Renderer2D* CreateRenderer2D(Renderer* renderer);
	};

}
