#include "ATPCH.h"
#include "IndexBuffer.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11IndexBuffer.h"

namespace Atom
{

	IndexBuffer* IndexBuffer::Create(void* indices, uint32_t count)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11IndexBuffer(indices, count);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

