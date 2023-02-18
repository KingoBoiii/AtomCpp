#include "ATPCH.h"
#include "VertexBuffer.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11VertexBuffer.h"

namespace Atom
{

	VertexBuffer* VertexBuffer::Create(const VertexBufferOptions& vertexBufferOptions)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11VertexBuffer(vertexBufferOptions);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

