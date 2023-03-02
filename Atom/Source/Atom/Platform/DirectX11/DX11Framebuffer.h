#pragma once
#include "Atom/Renderer/Framebuffer.h"

#include "DirectX11.h"

namespace Atom
{
	
	class DX11Framebuffer : public Framebuffer
	{
	public:
		DX11Framebuffer(const FramebufferOptions& framebufferOptions);
		virtual ~DX11Framebuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Clear() const override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int32_t value) const override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual int32_t ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const override;

		virtual void* GetColorAttachment(uint32_t attachmentIndex) const override;
		virtual void* GetImage() const override;
		
		virtual FramebufferOptions GetOptions() const override { return m_Options; }
	private:
		void Invalidate();

		void CreateColorAttachment(int32_t index, FramebufferTextureOptions colorAttachmentOptions);
		void CreateDepthStencilAttachment(FramebufferTextureOptions depthStencilAttachmentOptions);
		void CreateShaderResourceViews();

#if 0
		void CreateRenderTargetView();
		void CreateDepthStencilView();
		void CreateShaderResourceView();
#endif

		void SetD3D11Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	private:
		FramebufferOptions m_Options;

		std::vector<FramebufferTextureOptions> m_ColorAttachments;
		FramebufferTextureOptions m_DepthAttachment = FramebufferTextureFormat::None;

		std::vector<ID3D11Texture2D*> m_ColorAttachmentTextures = std::vector<ID3D11Texture2D*>();
		std::vector<ID3D11RenderTargetView*> m_ColorAttachmentRenderTargets = std::vector<ID3D11RenderTargetView*>();
		std::vector<ID3D11ShaderResourceView*> m_ColorAttachmentViews = std::vector<ID3D11ShaderResourceView*>();
		ID3D11DepthStencilView* m_DepthStencilAttachment = nullptr;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;

		D3D11_VIEWPORT m_Viewport;
#if 0
		ID3D11Texture2D* m_RenderTargetViewTexture = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
#endif

		ID3D11RenderTargetView* m_OldRenderTargetView = nullptr;
		ID3D11DepthStencilView* m_OldDepthStencilView = nullptr;
		D3D11_VIEWPORT m_OldViewport;
	};

}
