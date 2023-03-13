#include "ATPCH.h"
#include "DX11VertexBuffer.h"
#include "DX11RendererContext.h"

namespace Atom
{

	DX11VertexBuffer::DX11VertexBuffer(const VertexBufferSpecification& specification)
		: m_Specification(specification)
	{
		DX11RendererContext& context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate();
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
		ReleaseCOM(m_Buffer);

		m_Device = nullptr;
		m_DeviceContext = nullptr;
	}

	void DX11VertexBuffer::SetData(void* vertices, uint32_t size)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		m_DeviceContext->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, vertices, size);
		m_DeviceContext->Unmap(m_Buffer, 0);
	}

	void DX11VertexBuffer::Bind() const
	{
		m_DeviceContext->IASetVertexBuffers(0, 1, &m_Buffer, &m_Specification.Stride, &m_Specification.Offset);
	}

	void DX11VertexBuffer::Invalidate()
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = m_Specification.Size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		if(m_Specification.Vertices)
		{
			D3D11_SUBRESOURCE_DATA subResourceData = {};
			subResourceData.pSysMem = m_Specification.Vertices;
			subResourceData.SysMemPitch = 0;
			subResourceData.SysMemSlicePitch = 0;

			HRESULT hr = m_Device->CreateBuffer(&bufferDesc, &subResourceData, &m_Buffer);
			AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
		}
		else
		{
			HRESULT hr = m_Device->CreateBuffer(&bufferDesc, nullptr, &m_Buffer);
			AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
		}
	}

}