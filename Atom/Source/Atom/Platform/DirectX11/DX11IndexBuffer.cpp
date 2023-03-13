#include "ATPCH.h"
#include "DX11IndexBuffer.h"
#include "DX11RendererContext.h"

#include "DirectX11Utils.h"

namespace Atom
{

	DX11IndexBuffer::DX11IndexBuffer(const IndexBufferSpecification& specification)
		: m_Specification(specification)
	{
		DX11RendererContext& context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate();
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
		ReleaseCOM(m_Buffer);

		m_Device = nullptr;
		m_DeviceContext = nullptr;
	}

	void DX11IndexBuffer::Bind() const
	{
		// TODO: Move format into specification...
		m_DeviceContext->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Invalidate()
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = Utils::AtomUsageToD3D11(m_Specification.Usage);
		bufferDesc.ByteWidth = m_Specification.IndexCount * sizeof(uint32_t);
		bufferDesc.BindFlags = Utils::AtomBindFlagsToD3D11(m_Specification.BindFlags);
		bufferDesc.CPUAccessFlags = Utils::AtomCPUAccessToD3D11(m_Specification.CPUAccess);
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pSysMem = m_Specification.Indices;
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;

		HRESULT hr = m_Device->CreateBuffer(&bufferDesc, &subResourceData, &m_Buffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create index buffer");
	}

}