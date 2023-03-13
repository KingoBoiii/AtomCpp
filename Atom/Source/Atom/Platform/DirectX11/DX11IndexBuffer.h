#pragma once
#include "Atom/Renderer/IndexBuffer.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11IndexBuffer : public IndexBuffer
	{
	public:
		DX11IndexBuffer(const IndexBufferSpecification& specification);
		virtual ~DX11IndexBuffer();

		virtual void Bind() const override;

		virtual uint32_t GetIndexCount() const override { return m_Specification.IndexCount; }
	private:
		void Invalidate();
	private:
		IndexBufferSpecification m_Specification;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11Buffer* m_Buffer = nullptr;
	};

}
