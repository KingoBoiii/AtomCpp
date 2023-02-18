#include "ATPCH.h"
#include "UniformBuffer.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11UniformBuffer.h"

namespace Atom
{

	UniformBuffer* UniformBuffer::Create(const void* data, uint32_t size)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11UniformBuffer(data, size);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}