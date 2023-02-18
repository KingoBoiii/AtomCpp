#pragma once

namespace Atom
{

	class UniformBuffer
	{
	public:
		static UniformBuffer* Create(const void* data, uint32_t size, uint32_t slot = 0);
	public:
		virtual void SetData(const void* data, uint32_t size) const = 0;
		
		virtual void Bind() const = 0;
	};

}
