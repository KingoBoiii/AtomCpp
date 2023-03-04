#include "ATPCH.h"
#include "DX11Renderer.h"
#include "DX11RendererContext.h"
#include "DX11SwapChain.h"

#include "Atom/Display/Window.h"

namespace Atom
{

#define BLENDING_TEST

	DX11Renderer::DX11Renderer(Window* window)
		: m_Window(window), m_Viewport({ 0.0f, 0.0f, (float)window->GetWidth(), (float)window->GetHeight(), 0.0f, 1.0f})
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

		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, NULL);
		m_DeviceContext->OMSetBlendState(m_BlendState, NULL, 0xffffffff);
		
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

	void DX11Renderer::CreateBlendState()
	{
		D3D11_BLEND_DESC blendStateDesc;
		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
#ifndef BLENDING_TEST
		blendStateDesc.RenderTarget[0].BlendEnable = false;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
#else
		blendStateDesc.AlphaToCoverageEnable = false;
		blendStateDesc.IndependentBlendEnable = false;

		blendStateDesc.RenderTarget[0].BlendEnable = true;

		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
#endif

		HRESULT hr = m_Device->CreateBlendState(&blendStateDesc, &m_BlendState);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Blend State");
	}

	void DX11Renderer::CreateDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
#ifndef BLENDING_TEST
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xff;
		depthStencilDesc.StencilWriteMask = 0xff;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
#else
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xff;
		depthStencilDesc.StencilWriteMask = 0xff;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
#endif

		HRESULT hr = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Depth Stencil State");
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

