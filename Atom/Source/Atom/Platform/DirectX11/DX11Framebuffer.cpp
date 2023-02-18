#include "ATPCH.h"
#include "DX11Framebuffer.h"
#include "DX11RendererContext.h"

namespace Atom
{
	DX11Framebuffer::DX11Framebuffer(const FramebufferOptions& framebufferOptions)
		: m_Options(framebufferOptions), m_Viewport({ 0.0f, 0.0f, (float)framebufferOptions.Width, (float)framebufferOptions.Height, 0.0f, 1.0f })
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		Invalidate();
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_ShaderResourceView);

		m_Device = nullptr;
		m_DeviceContext = nullptr;
	}

	void DX11Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		AT_CORE_ASSERT(!(width <= 0 || height <= 0));

		m_Options.Width = width;
		m_Options.Height = height;

		Invalidate();
	}

	void DX11Framebuffer::Clear() const
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, m_Options.ClearColor);
		
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11Framebuffer::Bind() const
	{
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	void DX11Framebuffer::Unbind()
	{
		CreateShaderResourceView();
	}

	void* DX11Framebuffer::GetImage() const
	{
		return static_cast<void*>(m_ShaderResourceView);
	}

	void DX11Framebuffer::Invalidate()
	{
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_RenderTargetViewTexture);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_ShaderResourceView);

		CreateRenderTargetView();
		CreateDepthStencilView();
	}

	void DX11Framebuffer::CreateRenderTargetView()
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Width = m_Options.Width;
		desc.Height = m_Options.Height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr = m_Device->CreateTexture2D(&desc, NULL, &m_RenderTargetViewTexture);
		AT_CORE_ASSERT(!FAILED(hr), "Failed to create framebuffer texture");

		D3D11_RENDER_TARGET_VIEW_DESC desc1;
		ZeroMemory(&desc1, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc1.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		hr = m_Device->CreateRenderTargetView(m_RenderTargetViewTexture, &desc1, &m_RenderTargetView);
		AT_CORE_ASSERT(!FAILED(hr), "Failed to create framebuffer view");
	}

	void DX11Framebuffer::CreateDepthStencilView()
	{
		ID3D11Texture2D* depthStencilBuffer = nullptr;

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = m_Options.Width;
		depthStencilDesc.Height = m_Options.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		HRESULT hr = m_Device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
		AT_CORE_ASSERT(!FAILED(hr), "Failed to create Depth Stencil Buffer");

		hr = m_Device->CreateDepthStencilView(depthStencilBuffer, 0, &m_DepthStencilView);
		AT_CORE_ASSERT(!FAILED(hr), "Failed to create Depth Stencil View");

		ReleaseCOM(depthStencilBuffer);
	}

	void DX11Framebuffer::CreateShaderResourceView()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		
		ReleaseCOM(m_ShaderResourceView);
		HRESULT hr = m_Device->CreateShaderResourceView(m_RenderTargetViewTexture, &desc, &m_ShaderResourceView);
		AT_CORE_ASSERT(!FAILED(hr), "Failed to create Shader Resource View");
	}

	void DX11Framebuffer::SetD3D11Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		m_Viewport.TopLeftX = static_cast<float>(x);
		m_Viewport.TopLeftY = static_cast<float>(y);
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
	}

}