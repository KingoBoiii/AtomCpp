#include "ATPCH.h"
#include "IndexBuffer.h"

#include "DirectX11/DX11IndexBuffer.h"

namespace Atom
{

	IndexBuffer::IndexBuffer(const IndexBufferOptions& indexBufferOptions)
		: m_IndexBufferOptions(indexBufferOptions)
	{
	}

	IndexBuffer* IndexBufferFactory::Create(const IndexBufferOptions& indexBufferOptions)
	{
		return new DX11IndexBuffer(indexBufferOptions);
	}

}