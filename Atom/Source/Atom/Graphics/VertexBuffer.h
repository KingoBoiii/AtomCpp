#pragma once
#include "BufferLayout.h"

namespace Atom
{

	struct ATOM_API VertexBufferOptions
	{
		void* Vertices;
		uint32_t Size;
		uint32_t Stride;
		uint32_t Offset;
	};

	class ATOM_API VertexBuffer
	{
	public:
		VertexBuffer(const VertexBufferOptions& vertexBufferOptions);
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
	protected:
		VertexBufferOptions m_VertexBufferOptions;
	};

	class ATOM_API VertexBufferFactory
	{
	public:
		static VertexBuffer* Create(const VertexBufferOptions& vertexBufferOptions);
	};

}
