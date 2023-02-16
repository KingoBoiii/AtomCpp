#include "ATPCH.h"
#include "DX11Renderer.h"
#include "DX11SwapChain.h"

namespace Atom
{

	DX11Renderer::DX11Renderer(const RendererOptions& rendererOptions)
		: Renderer(rendererOptions), m_Viewport({ 0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f })
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;
	}

	DX11Renderer::~DX11Renderer()
	{
	}

	void DX11Renderer::Initialize()
	{
		IDXGISwapChain* swapChain = static_cast<DX11SwapChain*>(m_RendererOptions.SwapChain)->m_SwapChain;

		CreateRenderTargetView(swapChain);
		CreateDepthStencilBufferAndView(swapChain);

		// Bind the depth stencil view to the output merger stage.
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	void DX11Renderer::Clear() const
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, m_RendererOptions.ClearColor);

		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11Renderer::RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, int indexCount) const
	{
		if(indexCount == 0)
		{
			indexCount = indexBuffer->GetIndexCount();
		}

		pipeline->Bind();
		vertexBuffer->Bind();
		indexBuffer->Bind();

		m_DeviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void DX11Renderer::RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, int indexCount) const
	{
		if(indexCount == 0)
		{
			indexCount = indexBuffer->GetIndexCount();
		}

		pipeline->Bind();
		vertexBuffer->Bind();
		indexBuffer->Bind();
		uniformBuffer->Bind(0);

		m_DeviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void DX11Renderer::CreateRenderTargetView(IDXGISwapChain* swapChain)
	{
		// Create the render target view
		ID3D11Texture2D* backBuffer = nullptr;
		HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to get back buffer from swap chain!");

		hr = m_Device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create render target view!");

		backBuffer->Release();
	}

	void DX11Renderer::CreateDepthStencilBufferAndView(IDXGISwapChain* swapChain)
	{
		// Get the swap chain description.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChain->GetDesc(&swapChainDesc);

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

		HRESULT hr = m_Device->CreateTexture2D(&desc, NULL, &m_DepthStencilBuffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create depth stencil buffer!");

		// Create the depth stencil view.
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &dsvDesc, &m_DepthStencilView);
		AT_CORE_ASSERT(SUCCEEDED(hr), "failed to create Depth Stencil View");

	}

}