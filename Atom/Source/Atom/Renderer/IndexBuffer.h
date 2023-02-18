#pragma once

namespace Atom
{

	class IndexBuffer
	{
	public:
		static IndexBuffer* Create(void* indices, uint32_t count);
	public:
		virtual void Bind() const = 0;

		virtual uint32_t GetIndexCount() const = 0;
	};

}
