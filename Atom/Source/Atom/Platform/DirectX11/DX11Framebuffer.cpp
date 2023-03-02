#include "ATPCH.h"
#include "DX11Framebuffer.h"
#include "DX11RendererContext.h"

namespace Atom
{

	namespace Utils
	{

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch(format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static DXGI_FORMAT AtomFBTextureFormatToD3DX11(FramebufferTextureFormat format)
		{
			switch(format)
			{
				case FramebufferTextureFormat::RGBA8:			return DXGI_FORMAT_R8G8B8A8_UNORM;
				case FramebufferTextureFormat::RED_INTEGER:		return DXGI_FORMAT_R32_SINT;
				case FramebufferTextureFormat::DEPTH24STENCIL8:	return DXGI_FORMAT_D24_UNORM_S8_UINT;
			}

			AT_CORE_ASSERT(false);
			return DXGI_FORMAT_UNKNOWN;
		}

	}

	DX11Framebuffer::DX11Framebuffer(const FramebufferOptions& framebufferOptions)
		: m_Options(framebufferOptions), m_Viewport({ 0.0f, 0.0f, (float)framebufferOptions.Width, (float)framebufferOptions.Height, 0.0f, 1.0f })
	{
		DX11RendererContext& context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		for(auto& attachment : m_Options.Attachments.Attachments)
		{
			if(!Utils::IsDepthFormat(attachment.TextureFormat))
			{
				m_ColorAttachments.emplace_back(attachment);
			}
			else
			{
				m_DepthAttachment = attachment;
			}
		}

		Invalidate();
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
#if 0
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_ShaderResourceView);
#endif

		m_Device = nullptr;
		m_DeviceContext = nullptr;
}

	void DX11Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		AT_CORE_ASSERT(!(width <= 0 || height <= 0));

		m_Options.Width = width;
		m_Options.Height = height;

		SetD3D11Viewport(0, 0, width, height);
		Invalidate();
	}

	void DX11Framebuffer::Clear() const
	{
		for(size_t i = 0; i < m_ColorAttachments.size(); i++)
		{
			m_DeviceContext->ClearRenderTargetView(m_ColorAttachmentRenderTargets[i], m_Options.ClearColor);
		}

		m_DeviceContext->ClearDepthStencilView(m_DepthStencilAttachment, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

#if 0
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, m_Options.ClearColor);

		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
#endif
	}

	void DX11Framebuffer::ClearAttachment(uint32_t attachmentIndex, int32_t value) const
	{
		AT_CORE_ASSERT(m_ColorAttachments.size() > attachmentIndex);

		float clearColor[4] = { static_cast<float>(value), 0.0f, 0.0f, 0.0f };
		m_DeviceContext->ClearRenderTargetView(m_ColorAttachmentRenderTargets[attachmentIndex], clearColor);
	}

	void DX11Framebuffer::Bind()
	{
		for(size_t i = 0; i < m_ColorAttachmentViews.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentViews[i]);
		}

		m_DeviceContext->OMGetRenderTargets(1, &m_OldRenderTargetView, &m_OldDepthStencilView);

		m_DeviceContext->OMSetRenderTargets(m_ColorAttachments.size(), m_ColorAttachmentRenderTargets.data(), m_DepthStencilAttachment);

		static uint32_t s_ViewportCount = 1;
		m_DeviceContext->RSGetViewports(&s_ViewportCount, &m_OldViewport);

		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	void DX11Framebuffer::Unbind()
	{
		//CreateShaderResourceView();
		CreateShaderResourceViews();

		m_DeviceContext->OMSetRenderTargets(1, &m_OldRenderTargetView, m_OldDepthStencilView);

		m_DeviceContext->RSSetViewports(1, &m_OldViewport);
	}

	int32_t DX11Framebuffer::ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const
	{
		AT_CORE_ASSERT(m_ColorAttachments.size() > attachmentIndex);

		return 0;
	}

	void* DX11Framebuffer::GetColorAttachment(uint32_t attachmentIndex) const
	{
		AT_CORE_ASSERT(m_ColorAttachments.size() > attachmentIndex);

		return m_ColorAttachmentViews[attachmentIndex];
	}

	void* DX11Framebuffer::GetImage() const
	{
		return static_cast<void*>(m_ColorAttachmentViews[0]);
		//return static_cast<void*>(m_ShaderResourceView);
	}

	void DX11Framebuffer::Invalidate()
	{
		for(size_t i = 0; i < m_ColorAttachmentTextures.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentTextures[i]);
		}
		for(size_t i = 0; i < m_ColorAttachmentRenderTargets.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentRenderTargets[i]);
		}
		for(size_t i = 0; i < m_ColorAttachmentViews.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentViews[i]);
		}
#if 0
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_RenderTargetViewTexture);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_ShaderResourceView);
#endif

		if(m_ColorAttachments.size() > 0)
		{
			m_ColorAttachmentTextures.resize(m_ColorAttachments.size());
			m_ColorAttachmentRenderTargets.resize(m_ColorAttachments.size());
			m_ColorAttachmentViews.resize(m_ColorAttachments.size());

			for(size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				CreateColorAttachment(i, m_ColorAttachments[i]);
			}
		}

		if(m_DepthAttachment.TextureFormat != FramebufferTextureFormat::None)
		{
			CreateDepthStencilAttachment(m_DepthAttachment);
		}

		//CreateRenderTargetView();
		//CreateDepthStencilView();
	}

	void DX11Framebuffer::CreateColorAttachment(int32_t index, FramebufferTextureOptions colorAttachmentOptions)
	{
		D3D11_TEXTURE2D_DESC colorAttachmentDesc;
		ZeroMemory(&colorAttachmentDesc, sizeof(D3D11_TEXTURE2D_DESC));

		colorAttachmentDesc.Width = m_Options.Width;
		colorAttachmentDesc.Height = m_Options.Height;
		colorAttachmentDesc.MipLevels = 1;
		colorAttachmentDesc.ArraySize = 1;
		colorAttachmentDesc.Format = Utils::AtomFBTextureFormatToD3DX11(colorAttachmentOptions.TextureFormat);
		colorAttachmentDesc.SampleDesc.Count = 1;
		colorAttachmentDesc.SampleDesc.Quality = 0;
		colorAttachmentDesc.Usage = D3D11_USAGE_DEFAULT;
		colorAttachmentDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		colorAttachmentDesc.CPUAccessFlags = 0;
		colorAttachmentDesc.MiscFlags = 0;

		HRESULT hr = m_Device->CreateTexture2D(&colorAttachmentDesc, nullptr, &m_ColorAttachmentTextures[index]);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Color Attachment Texture");

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		renderTargetViewDesc.Format = colorAttachmentDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hr = m_Device->CreateRenderTargetView(m_ColorAttachmentTextures[index], &renderTargetViewDesc, &m_ColorAttachmentRenderTargets[index]);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Color Attachment Render Target");
	}

	void DX11Framebuffer::CreateDepthStencilAttachment(FramebufferTextureOptions depthStencilAttachmentOptions)
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthStencilDesc.Width = m_Options.Width;
		depthStencilDesc.Height = m_Options.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = Utils::AtomFBTextureFormatToD3DX11(depthStencilAttachmentOptions.TextureFormat);
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		ID3D11Texture2D* depthStencilBuffer;
		HRESULT hr = m_Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Depth Stencil Texture");

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		hr = m_Device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilAttachment);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Depth Stencil View");
	}

	void DX11Framebuffer::CreateShaderResourceViews()
	{
		for(size_t i = 0; i < m_ColorAttachments.size(); i++)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			shaderResourceViewDesc.Format = Utils::AtomFBTextureFormatToD3DX11(m_ColorAttachments[i].TextureFormat);
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			HRESULT hr = m_Device->CreateShaderResourceView(m_ColorAttachmentTextures[i], &shaderResourceViewDesc, &m_ColorAttachmentViews[i]);
			AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Shader Resource View");
		}
	}

#if 0
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
#endif

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