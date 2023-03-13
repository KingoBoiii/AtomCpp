#pragma once
#include "Atom/Renderer/Framebuffer.h"

#include "DirectX11.h"

namespace Atom
{
	
	class DX11Framebuffer : public Framebuffer
	{
	public:
		DX11Framebuffer(const FramebufferSpecification& framebufferOptions);
		virtual ~DX11Framebuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Clear() const override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int32_t value) const override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual int32_t ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const override;

		virtual void* GetColorAttachment(uint32_t attachmentIndex) const override;
		virtual void* GetImage() const override;
		
		virtual FramebufferSpecification GetOptions() const override { return m_Specification; }
	private:
		void Invalidate();

		void CreateColorAttachment(int32_t index, FramebufferTextureSpecification colorAttachmentOptions);
		void CreateDepthStencilAttachment(FramebufferTextureSpecification depthStencilAttachmentOptions);
		void CreateShaderResourceViews();

		void SetD3D11Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	private:
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachments;
		FramebufferTextureSpecification m_DepthAttachment = TextureFormat::None;

		std::vector<Texture2D*> m_ColorAttachmentTextures = std::vector<Texture2D*>();
		std::vector<ID3D11RenderTargetView*> m_ColorAttachmentRenderTargets = std::vector<ID3D11RenderTargetView*>();
		std::vector<ID3D11ShaderResourceView*> m_ColorAttachmentViews = std::vector<ID3D11ShaderResourceView*>();
		ID3D11DepthStencilView* m_DepthStencilAttachment = nullptr;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;

		D3D11_VIEWPORT m_Viewport;

		ID3D11RenderTargetView* m_OldRenderTargetView = nullptr;
		ID3D11DepthStencilView* m_OldDepthStencilView = nullptr;
		D3D11_VIEWPORT m_OldViewport;
	};

}
