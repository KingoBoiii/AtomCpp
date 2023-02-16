#include "ATPCH.h"
#include "DX11Renderer.h"
#include "DX11SwapChain.h"

namespace Atom
{

	DX11Renderer::DX11Renderer(const RendererOptions& rendererOptions)
		: Renderer(rendererOptions)
	{
	}

	DX11Renderer::~DX11Renderer()
	{
	}

	void DX11Renderer::Initialize()
	{
		DX11RendererContext context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		// Create the render target view
		ID3D11Texture2D* backBuffer = nullptr;
		HRESULT hr = reinterpret_cast<DX11SwapChain*>(m_RendererOptions.SwapChain)->m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to get back buffer from swap chain!");

		hr = m_Device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create render target view!");
		
		backBuffer->Release();
		
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);
	}

	void DX11Renderer::Clear() const
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, m_RendererOptions.ClearColor);
	}

}