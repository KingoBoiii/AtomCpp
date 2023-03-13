#include "ATPCH.h"
#include "DX11Framebuffer.h"
#include "DX11RendererContext.h"

#include "DirectX11Utils.h"
#include "DX11Texture2D.h"

namespace Atom
{

	namespace Utils
	{

		static bool IsDepthFormat(TextureFormat format)
		{
			switch(format)
			{
				case TextureFormat::Depth24Stencil8:  return true;
			}

			return false;
		}

	}

	DX11Framebuffer::DX11Framebuffer(const FramebufferSpecification& specification)
		: m_Specification(specification), m_Viewport({ 0.0f, 0.0f, (float)specification.Width, (float)specification.Height, 0.0f, 1.0f })
	{
		DX11RendererContext& context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		for(auto& attachment : m_Specification.Attachments.Attachments)
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
		for(size_t i = 0; i < m_ColorAttachmentTextures.size(); i++)
		{
			delete m_ColorAttachmentTextures[i];
		}
		for(size_t i = 0; i < m_ColorAttachmentRenderTargets.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentRenderTargets[i]);
	}
		for(size_t i = 0; i < m_ColorAttachmentViews.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentViews[i]);
		}

		m_Device = nullptr;
		m_DeviceContext = nullptr;
}

	void DX11Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		AT_CORE_ASSERT(!(width <= 0 || height <= 0));

		m_Specification.Width = width;
		m_Specification.Height = height;

		SetD3D11Viewport(0, 0, width, height);
		Invalidate();
	}

	void DX11Framebuffer::Clear() const
	{
		for(size_t i = 0; i < m_ColorAttachments.size(); i++)
		{
			m_DeviceContext->ClearRenderTargetView(m_ColorAttachmentRenderTargets[i], m_Specification.ClearColor);
		}

		m_DeviceContext->ClearDepthStencilView(m_DepthStencilAttachment, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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
		CreateShaderResourceViews();

		m_DeviceContext->OMSetRenderTargets(1, &m_OldRenderTargetView, m_OldDepthStencilView);

		m_DeviceContext->RSSetViewports(1, &m_OldViewport);
	}

	int32_t DX11Framebuffer::ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const
	{
		AT_CORE_ASSERT(m_ColorAttachments.size() > attachmentIndex);

		Texture2DSpecification stagingTextureSpec;
		stagingTextureSpec.Width = m_Specification.Width;
		stagingTextureSpec.Height = m_Specification.Height;
		stagingTextureSpec.Format = m_ColorAttachments[attachmentIndex].TextureFormat;
		stagingTextureSpec.Usage = TextureUsage::Staging;
		stagingTextureSpec.CPUAccess = CPUAccess::Read;
		stagingTextureSpec.BindFlags = BindFlags::None;

		DX11Texture2D* stagingTexture = new DX11Texture2D(stagingTextureSpec);

		m_DeviceContext->CopyResource(stagingTexture->m_Texture, static_cast<DX11Texture2D*>(m_ColorAttachmentTextures[attachmentIndex])->m_Texture);

		D3D11_MAPPED_SUBRESOURCE msr;
		HRESULT hr = m_DeviceContext->Map(stagingTexture->m_Texture, 0, D3D11_MAP_READ, 0, &msr);
		AT_CORE_ASSERT(SUCCEEDED(hr));

		int offset = y * msr.RowPitch + x * 4;
		int32_t pixelData = *(const int32_t*)((const uint8_t*)msr.pData + offset);
		m_DeviceContext->Unmap(stagingTexture->m_Texture, 0);

		delete stagingTexture;

		return pixelData;
	}

	void* DX11Framebuffer::GetColorAttachment(uint32_t attachmentIndex) const
	{
		AT_CORE_ASSERT(m_ColorAttachments.size() > attachmentIndex);

		return m_ColorAttachmentViews[attachmentIndex];
	}

	void* DX11Framebuffer::GetImage() const
	{
		return m_ColorAttachmentTextures[0]->GetTextureHandle();
	}

	void DX11Framebuffer::Invalidate()
	{
		for(size_t i = 0; i < m_ColorAttachmentTextures.size(); i++)
		{
			delete m_ColorAttachmentTextures[i];
		}
		for(size_t i = 0; i < m_ColorAttachmentRenderTargets.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentRenderTargets[i]);
		}
		for(size_t i = 0; i < m_ColorAttachmentViews.size(); i++)
		{
			ReleaseCOM(m_ColorAttachmentViews[i]);
		}

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

		if(m_DepthAttachment.TextureFormat != TextureFormat::None)
		{
			CreateDepthStencilAttachment(m_DepthAttachment);
		}
	}

	void DX11Framebuffer::CreateColorAttachment(int32_t index, FramebufferTextureSpecification colorAttachmentOptions)
	{
		Texture2DSpecification colorAttachmentSpec;
		colorAttachmentSpec.Width = m_Specification.Width;
		colorAttachmentSpec.Height = m_Specification.Height;
		colorAttachmentSpec.MipLevels = 1;
		colorAttachmentSpec.SampleCount = 1;
		colorAttachmentSpec.SampleQuality = 0;
		colorAttachmentSpec.Format = colorAttachmentOptions.TextureFormat;
		colorAttachmentSpec.Usage = TextureUsage::Default;
		colorAttachmentSpec.BindFlags = BindFlags::ShaderResource | BindFlags::RenderTarget;

		m_ColorAttachmentTextures[index] = new DX11Texture2D(colorAttachmentSpec);

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		renderTargetViewDesc.Format = Utils::AtomTextureFormatToDXGI(colorAttachmentOptions.TextureFormat); // colorAttachmentDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		HRESULT hr = m_Device->CreateRenderTargetView(static_cast<DX11Texture2D*>(m_ColorAttachmentTextures[index])->m_Texture, &renderTargetViewDesc, &m_ColorAttachmentRenderTargets[index]);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Color Attachment Render Target");
	}

	void DX11Framebuffer::CreateDepthStencilAttachment(FramebufferTextureSpecification depthStencilAttachmentOptions)
	{
		Texture2DSpecification depthStencilAttachmentSpec;
		depthStencilAttachmentSpec.Width = m_Specification.Width;
		depthStencilAttachmentSpec.Height = m_Specification.Height;
		depthStencilAttachmentSpec.MipLevels = 1;
		depthStencilAttachmentSpec.SampleCount = 1;
		depthStencilAttachmentSpec.SampleQuality = 0;
		depthStencilAttachmentSpec.Format = depthStencilAttachmentOptions.TextureFormat;
		depthStencilAttachmentSpec.Usage = TextureUsage::Default;
		depthStencilAttachmentSpec.BindFlags = BindFlags::DepthStencil;

		DX11Texture2D* depthStencilTextureBuffer = new DX11Texture2D(depthStencilAttachmentSpec);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		depthStencilViewDesc.Format = Utils::AtomTextureFormatToDXGI(depthStencilAttachmentOptions.TextureFormat);
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		HRESULT hr = m_Device->CreateDepthStencilView(depthStencilTextureBuffer->m_Texture, &depthStencilViewDesc, &m_DepthStencilAttachment);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Depth Stencil View");
	}

	void DX11Framebuffer::CreateShaderResourceViews()
	{
		for(size_t i = 0; i < m_ColorAttachmentTextures.size(); i++)
		{
			((DX11Texture2D*)m_ColorAttachmentTextures[i])->CreateShaderResourceView();
		}
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