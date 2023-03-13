#pragma once
#include "Atom/Core/Base.h"
#include "Atom/Renderer/Texture2D.h"

#include "DirectX11.h"

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
				case TextureFilter::NONE:
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
				case TextureWrap::NONE:
				default: break;
			}
			
			return D3D11_TEXTURE_ADDRESS_WRAP;
		}

		static D3D11_USAGE AtomTextureUsageToD3D11(TextureUsage usage)
		{
			switch(usage)
			{
				case Atom::TextureUsage::Static:		return D3D11_USAGE_DEFAULT;
				case Atom::TextureUsage::Dynamic:		return D3D11_USAGE_DYNAMIC;
				case Atom::TextureUsage::Staging:		return D3D11_USAGE_STAGING;
				case Atom::TextureUsage::NONE:
				default: break;
			}

			return D3D11_USAGE_DEFAULT;
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
				case TextureFormat::NONE:
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
				case TextureFormat::RedInteger:			return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::LuminanceAlpha:		return DXGI_FORMAT_R8G8B8A8_UNORM;
				case TextureFormat::NONE:
				default: break;
			}
			return DXGI_FORMAT_UNKNOWN;
		}
		
	}

}
