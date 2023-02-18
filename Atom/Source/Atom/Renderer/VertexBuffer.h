#pragma once

namespace Atom
{
	
	struct VertexBufferOptions
	{
		uint32_t Size;
		void* Vertices = nullptr;
		uint32_t Stride;
		uint32_t Offset;
	};

	class VertexBuffer
	{
	public:
		static VertexBuffer* Create(const VertexBufferOptions& vertexBufferOptions);
	public:
		virtual void SetData(void* vertices, uint32_t size) = 0;

		virtual void Bind() const = 0;
	};

}
