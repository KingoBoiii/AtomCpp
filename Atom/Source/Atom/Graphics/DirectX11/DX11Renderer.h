#pragma once
#include "Atom/Graphics/Renderer.h"

#include "DX11RendererContext.h"
#include "DirectX11.h"

namespace Atom
{

	class ATOM_API DX11Renderer : public Renderer
	{
	public:
		DX11Renderer(const RendererOptions& rendererOptions);
		virtual ~DX11Renderer();
		
		virtual void Initialize() override;

		virtual void Clear() const override;

		virtual void RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, int indexCount = 0) const override;
		virtual void RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, int indexCount = 0) const override;
	private:
		void CreateRenderTargetView(IDXGISwapChain* swapChain);
		void CreateDepthStencilBufferAndView(IDXGISwapChain* swapChain);
	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
		D3D11_VIEWPORT m_Viewport;
	};

}
