#pragma once
#include "Flags/BindFlags.h"
#include "Flags/CPUAccess.h"
#include "Flags/Usage.h"

namespace Atom
{

	struct IndexBufferSpecification
	{
		void* Indices;
		uint32_t IndexCount;

		Usage Usage = Usage::Default;
		CPUAccess CPUAccess = CPUAccess::None;
		const BindFlags BindFlags = BindFlags::IndexBuffer;
	};

	class IndexBuffer
	{
	public:
		static IndexBuffer* Create(const IndexBufferSpecification& specification);
	public:
		virtual void Bind() const = 0;

		virtual uint32_t GetIndexCount() const = 0;
	};

}
