#include "ATPCH.h"
#include "DX11RendererContext.h"

namespace Atom
{

	namespace Utils
	{

		static std::string GetFeatureLevelAsString(D3D_FEATURE_LEVEL featureLevel)
		{
			switch(featureLevel)
			{
				case D3D_FEATURE_LEVEL_9_1: return "Direct 3D 9.1";
				case D3D_FEATURE_LEVEL_9_2: return "Direct 3D 9.2";
				case D3D_FEATURE_LEVEL_9_3: return "Direct 3D 9.3";
				case D3D_FEATURE_LEVEL_10_0: return "Direct 3D 10";
				case D3D_FEATURE_LEVEL_10_1: return "Direct 3D 10.1";
				case D3D_FEATURE_LEVEL_11_0: return "Direct 3D 11";
				case D3D_FEATURE_LEVEL_11_1: return "Direct 3D 11.1";
				case D3D_FEATURE_LEVEL_12_0: return "Direct 3D 12";
				case D3D_FEATURE_LEVEL_12_1: return "Direct 3D 12.1";
				default: return "Unknown feature level";
			}
		}

		static std::string GetVendor(uint32_t vendorId)
		{
			switch(vendorId)
			{
				case 4318: return "NVIDIA Corporation";
				default: return std::to_string(vendorId);
			}
		}

	}

	DX11RendererContext* DX11RendererContext::s_Instance = nullptr;

	DX11RendererContext::DX11RendererContext(Window* window)
		: RendererContext(window)
	{
		s_Instance = this;
	}

	DX11RendererContext::~DX11RendererContext()
	{
		ReleaseCOM(m_Device);
		ReleaseCOM(m_DeviceContext);
	}

	void DX11RendererContext::Initialize()
	{
		UINT deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED; //  D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(AT_DEBUG)
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, NULL, 0, D3D11_SDK_VERSION, &device, &m_FeatureLevel, &deviceContext);
		AT_CORE_ASSERT(!FAILED(hr), "Failed to create DirectX 11 device");

		m_Device = reinterpret_cast<ID3D11Device4*>(device);
		m_DeviceContext = reinterpret_cast<ID3D11DeviceContext4*>(deviceContext);

		CreateRendererContextInfo();
	}

	void DX11RendererContext::CreateRendererContextInfo()
	{
		IDXGIFactory* factory;
		CreateDXGIFactory(IID_PPV_ARGS(&factory));

		IDXGIAdapter* adapter;
		AT_CORE_ASSERT(factory->EnumAdapters(0, &adapter) != DXGI_ERROR_NOT_FOUND, "Failed to find a suitable GPU for DirectX 11");

		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		char* renderer = reinterpret_cast<char*>(malloc(sizeof(desc.Description)));
		wcstombs(renderer, desc.Description, sizeof(desc.Description));

		m_RendererContextInfo.RendererAPI = "DirectX 11";
		m_RendererContextInfo.RendererName = renderer;
		m_RendererContextInfo.VendorName = Utils::GetVendor(desc.VendorId);

		PrintRendererContextInfo();
	}

}