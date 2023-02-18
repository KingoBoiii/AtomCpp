#include "ATPCH.h"
#include "DX11Renderer.h"
#include "DX11RendererContext.h"
#include "DX11SwapChain.h"

#include "Atom/Display/Window.h"

namespace Atom
{

	DX11Renderer::DX11Renderer(Window* window)
		: m_Window(window), m_Viewport({ 0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f })
	{
		m_SwapChain = static_cast<DX11SwapChain*>(m_Window->GetSwapChain());

		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;
	}

	DX11Renderer::~DX11Renderer()
	{
		m_SwapChain = nullptr;

		m_Device = nullptr;
		m_DeviceContext = nullptr;
	}

	void DX11Renderer::Initialize()
	{
		SetD3D11Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	}

	void DX11Renderer::Shutdown()
	{
	}

	void DX11Renderer::BeginFrame()
	{
		m_DeviceContext->OMSetRenderTargets(1, &m_SwapChain->m_RenderTargetView, m_SwapChain->m_DepthStencilView);
		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	void DX11Renderer::EndFrame()
	{
	}

	void DX11Renderer::Clear() const
	{
		m_DeviceContext->ClearRenderTargetView(m_SwapChain->m_RenderTargetView, new float[4] { 0.1f, 0.1f, 0.1f, 1.0f });
		m_DeviceContext->ClearDepthStencilView(m_SwapChain->m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11Renderer::RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t indexCount) const
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

	void DX11Renderer::RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, uint32_t indexCount) const
	{
		if(indexCount == 0)
		{
			indexCount = indexBuffer->GetIndexCount();
		}

		pipeline->Bind();
		vertexBuffer->Bind();
		indexBuffer->Bind();
		uniformBuffer->Bind();

		m_DeviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void DX11Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		SetD3D11Viewport(x, y, width, height);
		
		m_SwapChain->Resize(width, height);
	}

	void DX11Renderer::SetD3D11Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		m_Viewport.TopLeftX = static_cast<float>(x);
		m_Viewport.TopLeftY = static_cast<float>(y);
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
	}

}

