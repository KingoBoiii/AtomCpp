#include "ATPCH.h"
#include "DX11RendererContext.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace Atom
{

	DX11RendererContext* DX11RendererContext::s_Instance = nullptr;

	DX11RendererContext::DX11RendererContext(Window* window)
		: m_Window(window)
	{
		s_Instance = this;
	}

	DX11RendererContext::~DX11RendererContext()
	{
		ReleaseCOM(m_Device);
		ReleaseCOM(m_DeviceContext);

		s_Instance = nullptr;
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
		AT_CORE_WARN("Create Renderer Context Info");
	}

}
