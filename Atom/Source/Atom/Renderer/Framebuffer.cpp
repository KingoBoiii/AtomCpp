#include "ATPCH.h"
#include "Framebuffer.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11Framebuffer.h"

namespace Atom
{

	Framebuffer* Framebuffer::Create(const FramebufferSpecification& specification)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Framebuffer(specification);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}