#pragma once
#include "Flags/BindFlags.h"
#include "Flags/CPUAccess.h"
#include "Flags/Usage.h"

namespace Atom
{
	
	struct VertexBufferSpecification
	{
		uint32_t Size;
		void* Vertices = nullptr;
		uint32_t Stride;
		uint32_t Offset;

		Usage Usage = Usage::Default;
		CPUAccess CPUAccess = CPUAccess::None;
		const BindFlags BindFlags = BindFlags::VertexBuffer;
	};

	class VertexBuffer
	{
	public:
		static VertexBuffer* Create(const VertexBufferSpecification& specification);
	public:
		virtual void SetData(void* vertices, uint32_t size) = 0;

		virtual void Bind() const = 0;
	};

}
