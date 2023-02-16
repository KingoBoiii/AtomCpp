#pragma once
#include "Atom/Graphics/IndexBuffer.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11IndexBuffer : public IndexBuffer
	{
	public:
		DX11IndexBuffer(const IndexBufferOptions& indexBufferOptions);
		virtual ~DX11IndexBuffer();

		virtual void Bind() const override;
	private:
		void Invalidate();
	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11Buffer* m_Buffer = nullptr;
	};

}
