#pragma once
#include "Atom/Core/Base.h"
#include "Atom/Renderer/Texture2D.h"

#include <d3d11_4.h>
#pragma comment(lib,"d3d11.lib")

namespace Atom
{

	namespace Utils
	{

		static D3D11_FILTER AtomFilterToD3D11(TextureFilter filter)
		{
			switch(filter)
			{
				case TextureFilter::Nearest:			return D3D11_FILTER_MIN_MAG_MIP_POINT;
				case TextureFilter::Linear:				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				case TextureFilter::None:
				default: break;
			}

			return D3D11_FILTER_ANISOTROPIC;
		}

		static D3D11_TEXTURE_ADDRESS_MODE AtomTextureWrapToD3D11(TextureWrap wrap)
		{
			switch(wrap)
			{
				case TextureWrap::Clamp:				return D3D11_TEXTURE_ADDRESS_CLAMP;
				case TextureWrap::ClampToBorder:		return D3D11_TEXTURE_ADDRESS_CLAMP;
				case TextureWrap::ClampToEdge:			return D3D11_TEXTURE_ADDRESS_CLAMP;
				case TextureWrap::Repeat:				return D3D11_TEXTURE_ADDRESS_WRAP;
				case TextureWrap::MirroredRepeat:		return D3D11_TEXTURE_ADDRESS_MIRROR;
				case TextureWrap::None:
				default: break;
			}

			return D3D11_TEXTURE_ADDRESS_WRAP;
		}

		static D3D11_USAGE AtomUsageToD3D11(Usage usage)
		{
			switch(usage)
			{
				case Usage::Static:						return D3D11_USAGE_DEFAULT;
				case Usage::Dynamic:					return D3D11_USAGE_DYNAMIC;
				case Usage::Staging:					return D3D11_USAGE_STAGING;
				case Usage::None:
				default: break;
			}

			return D3D11_USAGE_DEFAULT;
		}

		static uint32_t AtomCPUAccessToD3D11(CPUAccess cpuAccess)
		{
			switch(cpuAccess)
			{
				case CPUAccess::Read:					return D3D11_CPU_ACCESS_READ;
				case CPUAccess::Write:					return D3D11_CPU_ACCESS_WRITE;
				case CPUAccess::None:
				default: break;
			}

			return 0;
		}

		static uint32_t AtomBindFlagsToD3D11(BindFlags bindFlags)
		{
			uint32_t d3d11BindFlags = 0;

			if((uint32_t)bindFlags & (uint32_t)BindFlags::VertexBuffer)
				d3d11BindFlags |= D3D11_BIND_VERTEX_BUFFER;

			if((uint32_t)bindFlags & (uint32_t)BindFlags::IndexBuffer)
				d3d11BindFlags |= D3D11_BIND_INDEX_BUFFER;

			if((uint32_t)bindFlags & (uint32_t)BindFlags::ConstantBuffer)
				d3d11BindFlags |= D3D11_BIND_CONSTANT_BUFFER;

			if((uint32_t)bindFlags & (uint32_t)BindFlags::ShaderResource)
				d3d11BindFlags |= D3D11_BIND_SHADER_RESOURCE;

			if((uint32_t)bindFlags & (uint32_t)BindFlags::RenderTarget)
				d3d11BindFlags |= D3D11_BIND_RENDER_TARGET;

			if((uint32_t)bindFlags & (uint32_t)BindFlags::DepthStencil)
				d3d11BindFlags |= D3D11_BIND_DEPTH_STENCIL;

			if((uint32_t)bindFlags & (uint32_t)BindFlags::None)
				d3d11BindFlags = 0;

			return d3d11BindFlags;
		}

		// TODO: Move
		static int32_t AtomTextureChannelsByFormat(TextureFormat format)
		{
			switch(format)
			{
				case TextureFormat::RGBA:				return 4;
				case TextureFormat::RGB:				return 3;
				case TextureFormat::Red:				return 4;
				case TextureFormat::RedInteger:			return 4;
				case TextureFormat::LuminanceAlpha:		return 4;
				case TextureFormat::Depth24Stencil8:	return 4;
				case TextureFormat::None:
				default: break;
			}

			AT_CORE_ASSERT(false, "Unknown Texture Format!");
			return 0;
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

		// TODO: Move to DXGIUtils?
		static DXGI_FORMAT AtomTextureFormatToDXGI(TextureFormat format)
		{
			switch(format)
			{
				case TextureFormat::RGBA:				return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::RGB:				return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				case TextureFormat::Red:				return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::RedInteger:			return DXGI_FORMAT_R32_SINT;
				case TextureFormat::LuminanceAlpha:		return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::Depth24Stencil8:	return DXGI_FORMAT_D24_UNORM_S8_UINT;
				case TextureFormat::None:
				default: break;
			}
			return DXGI_FORMAT_UNKNOWN;
		}

	}

}
