#include "ATPCH.h"
#include "DX11VertexBuffer.h"

#include "DX11RendererContext.h"

namespace Atom
{
	DX11VertexBuffer::DX11VertexBuffer(const VertexBufferOptions& vertexBufferOptions)
		: VertexBuffer(vertexBufferOptions)
	{
		Invalidate();
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
	}

	void DX11VertexBuffer::Bind() const
	{
		m_DeviceContext->IASetVertexBuffers(0, 1, &m_Buffer, &m_VertexBufferOptions.Stride, &m_VertexBufferOptions.Offset);
	}

	void DX11VertexBuffer::Invalidate()
	{
		DX11RendererContext context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = m_VertexBufferOptions.Size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pSysMem = m_VertexBufferOptions.Vertices;
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;

		HRESULT hr = m_Device->CreateBuffer(&bufferDesc, &subResourceData, &m_Buffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
	}

}