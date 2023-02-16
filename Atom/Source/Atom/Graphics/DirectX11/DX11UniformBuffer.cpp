#include "ATPCH.h"
#include "DX11UniformBuffer.h"
#include "DX11RendererContext.h"

namespace Atom
{

	DX11UniformBuffer::DX11UniformBuffer(const void* data, uint32_t size)
	{
		DX11RendererContext& context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate(data, size);
	}

	DX11UniformBuffer::~DX11UniformBuffer()
	{
		ReleaseCOM(m_Buffer);
	}

	void DX11UniformBuffer::SetData(const void* data, uint32_t size) const
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		m_DeviceContext->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, data, size);
		m_DeviceContext->Unmap(m_Buffer, 0);
	}

	void DX11UniformBuffer::Bind(uint32_t slot) const
	{
		m_DeviceContext->VSSetConstantBuffers(slot, 1, &m_Buffer);
		//m_DeviceContext->PSSetConstantBuffers(slot, 1, &m_Buffer);
	}

	void DX11UniformBuffer::Invalidate(const void* data, uint32_t size)
	{
		D3D11_BUFFER_DESC bufferDesc = { };
		bufferDesc.ByteWidth = size;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData = { };
		subresourceData.pSysMem = data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT result = m_Device->CreateBuffer(&bufferDesc, &subresourceData, &m_Buffer);
		AT_CORE_ASSERT(SUCCEEDED(result), "Failed to create uniform buffer!");
	}

}