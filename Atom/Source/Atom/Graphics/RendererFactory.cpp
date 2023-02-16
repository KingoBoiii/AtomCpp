#include "ATPCH.h"
#include "RendererFactory.h"

#include "Renderer2D.h"

#include "DirectX11/DX11Renderer.h"

namespace Atom
{

	Renderer* RendererFactory::CreateRenderer(const RendererOptions& rendererOptions)
	{
		return new DX11Renderer(rendererOptions);
	}

	Renderer2D* RendererFactory::CreateRenderer2D(Renderer* renderer)
	{
		return new Renderer2D(renderer);
	}

}