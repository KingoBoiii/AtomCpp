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
		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual void* GetImage() const override;
	private:
		void Invalidate();

		void CreateRenderTargetView();
		void CreateDepthStencilView();
		void CreateShaderResourceView();

		void SetD3D11Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	private:
		FramebufferOptions m_Options;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;

		D3D11_VIEWPORT m_Viewport;
		ID3D11Texture2D* m_RenderTargetViewTexture = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
	};

}
