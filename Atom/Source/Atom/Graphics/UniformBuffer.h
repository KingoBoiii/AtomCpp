#pragma once

namespace Atom
{

	class ATOM_API UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, uint32_t size) const = 0;
		
		virtual void Bind(uint32_t slot) const = 0;
	};

	class ATOM_API UniformBufferFactory
	{
	public:
		static UniformBuffer* Create(const void* data, uint32_t size);
	};

}
