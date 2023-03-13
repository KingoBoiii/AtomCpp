#include "ATPCH.h"
#include "DX11Texture2D.h"
#include "DX11RendererContext.h"

#include "DirectX11Utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Atom
{

	DX11Texture2D::DX11Texture2D(const Texture2DSpecification& specification)
		: m_Options(specification), m_Channels(Utils::AtomTextureChannelsByFormat(specification.Format))
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate();
	}

	DX11Texture2D::DX11Texture2D(const std::filesystem::path& filepath, const Texture2DSpecification& specification)
		: m_Filepath(filepath), m_Options(specification)
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		stbi_set_flip_vertically_on_load(m_Options.FlipVertically);
		uint8_t* data = stbi_load(m_Filepath.string().c_str(), (int32_t*)&m_Options.Width, (int32_t*)&m_Options.Height, (int32_t*)&m_Channels, 0);
		AT_CORE_ASSERT(data, "Failed to load image!");

		Invalidate(data);

		stbi_image_free(data);
	}

	DX11Texture2D::DX11Texture2D(uint32_t width, uint32_t height, const Texture2DSpecification& specification)
		: m_Options(specification)
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate(nullptr);
	}

	DX11Texture2D::~DX11Texture2D()
	{
		ReleaseCOM(m_Texture);
		ReleaseCOM(m_ShaderResourceView);
		ReleaseCOM(m_SamplerState);

		m_Device = nullptr;
		m_DeviceContext = nullptr;
	}

	void DX11Texture2D::Bind(uint32_t slot) const
	{
		m_DeviceContext->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
		m_DeviceContext->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void DX11Texture2D::SetData(void* data)
	{
		Invalidate(data);
	}

	void DX11Texture2D::Invalidate(void* data)
	{
		ReleaseCOM(m_Texture);
		ReleaseCOM(m_ShaderResourceView);
		ReleaseCOM(m_SamplerState);

		CreateTexture(data);
		CreateShaderResourceView();
		CreateSamplerState();
	}

	void DX11Texture2D::CreateTexture(void* data)
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

		desc.Width = m_Options.Width;
		desc.Height = m_Options.Height;
		desc.MipLevels = m_Options.MipLevels;
		desc.ArraySize = 1;
		desc.Format = Utils::AtomTextureFormatToDXGI(m_Options.Format);
		desc.SampleDesc.Count = m_Options.SampleCount;
		desc.SampleDesc.Quality = m_Options.SampleQuality;
		desc.Usage = Utils::AtomTextureUsageToD3D11(m_Options.Usage);
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if(m_Options.RenderTarget)
		{
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		desc.CPUAccessFlags = m_Options.MipLevels > 1 ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = m_Options.MipLevels > 1 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		HRESULT hr = 0;
		if(data)
		{
			D3D11_SUBRESOURCE_DATA initialData;
			ZeroMemory(&initialData, sizeof(D3D11_SUBRESOURCE_DATA));

			initialData.pSysMem = data;
			initialData.SysMemPitch = m_Options.Width * Utils::AtomTextureChannelsByFormat(m_Options.Format);
			initialData.SysMemSlicePitch = 0;
			hr = m_Device->CreateTexture2D(&desc, &initialData, &m_Texture);
		}
		else
		{
			hr = m_Device->CreateTexture2D(&desc, nullptr, &m_Texture);
		}

		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Texture2D");
	}

	void DX11Texture2D::CreateShaderResourceView()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		desc.Format = Utils::AtomTextureFormatToDXGI(m_Options.Format);
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2DArray.ArraySize = 1;
		desc.Texture2DArray.MipLevels = m_Options.MipLevels;

		HRESULT hr = m_Device->CreateShaderResourceView(m_Texture, &desc, &m_ShaderResourceView);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Shader Resource View for Texture2D");
	}

	void DX11Texture2D::CreateSamplerState()
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

		desc.AddressU = Utils::AtomTextureWrapToD3D11(m_Options.WrapMode);
		desc.AddressV = Utils::AtomTextureWrapToD3D11(m_Options.WrapMode);
		desc.AddressW = Utils::AtomTextureWrapToD3D11(m_Options.WrapMode);
		desc.MinLOD = 0;
		desc.MaxLOD = 11;
		desc.Filter = Utils::AtomFilterToD3D11(m_Options.Filter);
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = m_Device->CreateSamplerState(&desc, &m_SamplerState);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Sampler State for Texture2D");
	}

}