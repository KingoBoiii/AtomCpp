#pragma once

namespace Atom
{

	struct ATOM_API IndexBufferOptions
	{
		uint32_t* Indices;
		uint32_t Count;
	};

	class ATOM_API IndexBuffer
	{
	public:
		IndexBuffer(const IndexBufferOptions& indexBufferOptions);
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;

		uint32_t GetIndexCount() const { return m_IndexBufferOptions.Count; }
	protected:
		IndexBufferOptions m_IndexBufferOptions;
	};

	class ATOM_API IndexBufferFactory
	{
	public:
		static IndexBuffer* Create(const IndexBufferOptions& indexBufferOptions);
	};

}
