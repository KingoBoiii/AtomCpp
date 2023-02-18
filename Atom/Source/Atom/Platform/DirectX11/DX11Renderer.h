#pragma once
#include "Atom/Renderer/IRenderer.h"

#include <d3d11_4.h>
#include "DX11SwapChain.h"

namespace Atom
{
	
	class DX11Renderer : public IRenderer
	{
	public:
		DX11Renderer(Window* window);
		virtual ~DX11Renderer();

		virtual void Initialize() override;
		virtual void Shutdown() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void Clear() const override;

		virtual void RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t indexCount = 0) const override;
		virtual void RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, uint32_t indexCount = 0) const override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	private:
		void SetD3D11Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	private:
		Window* m_Window = nullptr;

		DX11SwapChain* m_SwapChain = nullptr;
		
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;

		D3D11_VIEWPORT m_Viewport;
	};
}
