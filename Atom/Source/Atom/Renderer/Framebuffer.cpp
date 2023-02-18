#include "ATPCH.h"
#include "Framebuffer.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11Framebuffer.h"

namespace Atom
{

	Framebuffer* Framebuffer::Create(const FramebufferOptions& framebufferOptions)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Framebuffer(framebufferOptions);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}