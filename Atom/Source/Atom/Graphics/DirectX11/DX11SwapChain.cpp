#include "ATPCH.h"
#include "DX11SwapChain.h"

#include "DX11RendererContext.h"

namespace Atom
{

	DX11SwapChain::DX11SwapChain(const SwapChainOptions& swapChainOptions)
		: SwapChain(swapChainOptions)
	{
	}

	DX11SwapChain::~DX11SwapChain()
	{
		if(m_SwapChain)
		{
			m_SwapChain->Release();
		}
	}

	void DX11SwapChain::Initialize()
	{
		CreateSwapChain();
	}

	void DX11SwapChain::Present() const
	{
		m_SwapChain->Present(1, 0);
	}

	void DX11SwapChain::CreateSwapChain()
	{
		DX11RendererContext& context = DX11RendererContext::Get();

		IDXGIFactory* dxgiFactory = nullptr;
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		HWND hwnd = static_cast<HWND>(m_SwapChainOptions.Window->GetNativeWindowHandle());

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Width = m_SwapChainOptions.Width;
		swapChainDesc.BufferDesc.Height = m_SwapChainOptions.Height;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = TRUE;

		dxgiFactory->CreateSwapChain(context.m_Device, &swapChainDesc, &m_SwapChain);

		dxgiFactory->Release();
	}

}