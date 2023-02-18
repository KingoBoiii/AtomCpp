#include "ATPCH.h"
#include "SwapChain.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11SwapChain.h"

namespace Atom
{

	SwapChain* SwapChain::Create(Window* window)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11SwapChain(window);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

