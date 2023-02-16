#pragma once
#include "Atom/Graphics/VertexBuffer.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11VertexBuffer : public VertexBuffer
	{
	public:
		DX11VertexBuffer(const VertexBufferOptions& vertexBufferOptions);
		virtual ~DX11VertexBuffer();

		virtual void Bind() const override;
	private:
		void Invalidate();
	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11Buffer* m_Buffer = nullptr;
	};

}