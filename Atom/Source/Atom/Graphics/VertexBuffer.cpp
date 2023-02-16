#include "ATPCH.h"
#include "VertexBuffer.h"

#include "DirectX11/DX11VertexBuffer.h"

namespace Atom
{

	VertexBuffer::VertexBuffer(const VertexBufferOptions& vertexBufferOptions)
		: m_VertexBufferOptions(vertexBufferOptions)
	{
	}

	VertexBuffer* VertexBufferFactory::Create(const VertexBufferOptions& vertexBufferOptions)
	{
		return new DX11VertexBuffer(vertexBufferOptions);
	}

}