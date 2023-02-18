#pragma once
#include "Atom/Renderer/VertexBuffer.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11VertexBuffer : public VertexBuffer
	{
	public:
		DX11VertexBuffer(const VertexBufferOptions& vertexBufferOptions);
		virtual ~DX11VertexBuffer();

		virtual void SetData(void* vertices, uint32_t size) override;

		virtual void Bind() const override;
	private:
		void Invalidate();
	private:
		VertexBufferOptions m_Options;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11Buffer* m_Buffer = nullptr;
	};

}