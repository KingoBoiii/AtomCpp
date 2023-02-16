#include "ATPCH.h"
#include "UniformBuffer.h"

#include "DirectX11/DX11UniformBuffer.h"

namespace Atom
{

	UniformBuffer* UniformBufferFactory::Create(const void* data, uint32_t size)
	{
		return new DX11UniformBuffer(data, size);
	}

}