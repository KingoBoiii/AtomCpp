#include "ATPCH.h"
#include "DX11IndexBuffer.h"

#include "DX11RendererContext.h"

namespace Atom
{

	DX11IndexBuffer::DX11IndexBuffer(const IndexBufferOptions& indexBufferOptions)
		: IndexBuffer(indexBufferOptions)
	{
		DX11RendererContext& context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate();
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
		ReleaseCOM(m_Buffer);
	}

	void DX11IndexBuffer::Bind() const
	{
		m_DeviceContext->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Invalidate()
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = m_IndexBufferOptions.Count * sizeof(uint32_t);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		
		D3D11_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pSysMem = m_IndexBufferOptions.Indices;
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;

		HRESULT hr = m_Device->CreateBuffer(&bufferDesc, &subResourceData, &m_Buffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create index buffer");
	}

}