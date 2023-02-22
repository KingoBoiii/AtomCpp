#include "ATPCH.h"
#include "DX11Texture2D.h"
#include "DX11RendererContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Atom
{

	namespace Utils
	{

		int32_t AtomTextureChannelsByFormat(TextureFormat format)
		{
			switch(format)
			{
				case TextureFormat::RGBA:				return 4;
				case TextureFormat::RGB:				return 3;
				case TextureFormat::RED:				return 4;
				case TextureFormat::RED_INTEGER:		return 4;
				case TextureFormat::LUMINANCE_ALPHA:	return 4;
				case TextureFormat::NONE:
				default: break;
			}
			return 0;
		}

		DXGI_FORMAT AtomTextureFormatToDX(TextureFormat format)
		{
			switch(format)
			{
				case TextureFormat::RGBA:				return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::RGB:				return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				case TextureFormat::RED:				return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::RED_INTEGER:		return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::LUMINANCE_ALPHA:	return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::NONE:
				default: break;
			}
			return DXGI_FORMAT_UNKNOWN;
		}

		D3D11_FILTER AtomFilterToDX(TextureFilter filter)
		{
			switch(filter)
			{
				case TextureFilter::NEAREST: return D3D11_FILTER_MIN_MAG_MIP_POINT;
				case TextureFilter::LINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				case TextureFilter::NONE:
				default: break;
			}
			return D3D11_FILTER_ANISOTROPIC;
		}

		D3D11_TEXTURE_ADDRESS_MODE AtomTextureWrapToDX(TextureWrap wrap)
		{
			switch(wrap)
			{
				case TextureWrap::CLAMP:				return D3D11_TEXTURE_ADDRESS_CLAMP;
				case TextureWrap::CLAMP_TO_BORDER:		return D3D11_TEXTURE_ADDRESS_CLAMP;
				case TextureWrap::CLAMP_TO_EDGE:		return D3D11_TEXTURE_ADDRESS_CLAMP;
				case TextureWrap::REPEAT:				return D3D11_TEXTURE_ADDRESS_WRAP;
				case TextureWrap::MIRRORED_REPEAT:		return D3D11_TEXTURE_ADDRESS_MIRROR;
				case TextureWrap::NONE:
				default: break;
			}
			return D3D11_TEXTURE_ADDRESS_WRAP;
		}

		static uint32_t GetMipLevels(uint32_t width, uint32_t height)
		{
			uint32_t levels = 1;
			while((width | height) >> levels)
			{
				levels++;
			}
			return levels;
		}

	}

	DX11Texture2D::DX11Texture2D(const std::filesystem::path& filepath, const Texture2DOptions& texture2DOptions)
		: m_Filepath(filepath), m_Options(texture2DOptions)
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		stbi_set_flip_vertically_on_load(m_Options.FlipVertically);
		uint8_t* data = stbi_load(m_Filepath.string().c_str(), (int32_t*)&m_Width, (int32_t*)&m_Height, (int32_t*)&m_Channels, 0);
		AT_CORE_ASSERT(data, "Failed to load image!");

		Invalidate(data);

		stbi_image_free(data);
	}

	DX11Texture2D::DX11Texture2D(uint32_t width, uint32_t height, const Texture2DOptions& texture2DOptions)
		: m_Width(width), m_Height(height), m_Options(texture2DOptions)
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate();
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

		bool generateMips = data != nullptr;
		uint32_t mipLevels = generateMips ? Utils::GetMipLevels(m_Width, m_Height) : 1;
		D3D11_USAGE usage = generateMips ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;

		CreateTexture(data, mipLevels, usage);
		CreateShaderResourceView(mipLevels);
		CreateSamplerState();

		if(generateMips)
		{
			AT_CORE_ASSERT(m_Texture);
			AT_CORE_ASSERT(m_ShaderResourceView);

			int rowPitch = m_Width * Utils::AtomTextureChannelsByFormat(m_Options.Format);// m_Channels; // *sizeof(uint8_t);
			m_DeviceContext->UpdateSubresource(m_Texture, 0, nullptr, data, rowPitch, 0);
			m_DeviceContext->GenerateMips(m_ShaderResourceView);
		}
	}

	void DX11Texture2D::CreateTexture(void* data, uint32_t mipLevels, D3D11_USAGE usage)
	{
		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));
		bool generateMips = data != nullptr;

		desc.Width = m_Width;
		desc.Height = m_Height;
		desc.MipLevels = mipLevels;
		desc.ArraySize = 1;
		desc.Format = Utils::AtomTextureFormatToDX(m_Options.Format);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = usage;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if(generateMips)
		{
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		desc.CPUAccessFlags = generateMips ? 0 : D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		HRESULT hr = m_Device->CreateTexture2D(&desc, nullptr, &m_Texture);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Texture2D");
	}

	void DX11Texture2D::CreateShaderResourceView(uint32_t mipLevels)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		memset(&desc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		desc.Format = Utils::AtomTextureFormatToDX(m_Options.Format);
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 1;
		desc.Texture2DArray.MipLevels = mipLevels;

		HRESULT hr = m_Device->CreateShaderResourceView(m_Texture, &desc, &m_ShaderResourceView);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Shader Resource View for Texture2D");
	}

	void DX11Texture2D::CreateSamplerState()
	{
		D3D11_SAMPLER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_SAMPLER_DESC));

		desc.AddressU = Utils::AtomTextureWrapToDX(m_Options.WrapMode);
		desc.AddressV = Utils::AtomTextureWrapToDX(m_Options.WrapMode);
		desc.AddressW = Utils::AtomTextureWrapToDX(m_Options.WrapMode);
		desc.MinLOD = 0;
		desc.MaxLOD = 11;
		desc.Filter = Utils::AtomFilterToDX(m_Options.Filter);
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = m_Device->CreateSamplerState(&desc, &m_SamplerState);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Sampler State for Texture2D");
	}

}