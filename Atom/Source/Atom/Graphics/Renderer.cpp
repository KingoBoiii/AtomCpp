#include "ATPCH.h"
#include "Renderer.h"

#include "Atom/Graphics/DirectX11/DX11Renderer.h"

namespace Atom
{

	Renderer::Renderer(const RendererOptions& rendererOptions)
		: m_RendererOptions(rendererOptions)
	{
	}

	Renderer* RendererFactory::Create(const RendererOptions& rendererOptions)
	{
		return new DX11Renderer(rendererOptions);
	}

}