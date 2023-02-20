#include "ATPCH.h"
#include "DX11SwapChain.h"
#include "DX11RendererContext.h"

#include "Atom/Display/Window.h"

namespace Atom
{
	
#define USE_MODERN_SWAPCHAIN 0

	DX11SwapChain::DX11SwapChain(Window* window)
		: m_Window(window)
	{
	}

	DX11SwapChain::~DX11SwapChain()
	{
		ReleaseCOM(m_SwapChain);

		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);
	}

	void DX11SwapChain::Initialize()
	{
		Invalidate();
	}

	void DX11SwapChain::Present() const
	{
		m_SwapChain->Present(1, 0);
	}

	void DX11SwapChain::Resize(uint32_t width, uint32_t height)
	{
		Invalidate();
	}

	void DX11SwapChain::Invalidate()
	{
		if(m_SwapChain)
		{
			ReleaseCOM(m_SwapChain);

			ReleaseCOM(m_RenderTargetView);
			ReleaseCOM(m_DepthStencilView);
			ReleaseCOM(m_DepthStencilBuffer);
		}

		DX11RendererContext& context = DX11RendererContext::Get();

		CreateSwapChain(context.m_Device);
		CreateRenderTargetView(context.m_Device);
		CreateDepthStencilBufferAndView(context.m_Device);
	}

	void DX11SwapChain::CreateSwapChain(ID3D11Device* device)
	{
		IDXGIFactory5* dxgiFactory = nullptr;
		CreateDXGIFactory(__uuidof(IDXGIFactory5), (void**)&dxgiFactory);

		HWND hwnd = static_cast<HWND>(m_Window->GetNativeWindowHandle());

#if USE_MODERN_SWAPCHAIN
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc1{ };
		swapChainDesc1.Width = m_Window->GetWidth();
		swapChainDesc1.Height = m_Window->GetHeight();
		swapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc1.Stereo = false;
		swapChainDesc1.SampleDesc.Count = 1;
		swapChainDesc1.SampleDesc.Quality = 0;
		swapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc1.BufferCount = 2;
		swapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc1.Flags = 0;

		IDXGISwapChain1* swapChain = nullptr;
		HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(device, hwnd, &swapChainDesc1, nullptr, nullptr, &swapChain);
		AT_CORE_ASSERT(SUCCEEDED(hr));

		m_SwapChain = swapChain;
#else
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Width = m_Window->GetWidth();
		swapChainDesc.BufferDesc.Height = m_Window->GetHeight();
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;

		HRESULT hr = dxgiFactory->CreateSwapChain(device, &swapChainDesc, &m_SwapChain);
		AT_CORE_ASSERT(SUCCEEDED(hr));
#endif

		ReleaseCOM(dxgiFactory);
	}

	void DX11SwapChain::CreateRenderTargetView(ID3D11Device* device)
	{
		// Create the render target view
		ID3D11Texture2D* backBuffer = nullptr;
		HRESULT hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to get back buffer from swap chain!");

		hr = device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create render target view!");

		backBuffer->Release();
	}

	void DX11SwapChain::CreateDepthStencilBufferAndView(ID3D11Device* device)
	{
		// Get the swap chain description.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		m_SwapChain->GetDesc(&swapChainDesc);

		// Create the depth stencil buffer.
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = swapChainDesc.BufferDesc.Width;
		desc.Height = swapChainDesc.BufferDesc.Height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = device->CreateTexture2D(&desc, NULL, &m_DepthStencilBuffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create depth stencil buffer!");

		// Create the depth stencil view.
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(m_DepthStencilBuffer, &dsvDesc, &m_DepthStencilView);
		AT_CORE_ASSERT(SUCCEEDED(hr), "failed to create Depth Stencil View");
	}

}

