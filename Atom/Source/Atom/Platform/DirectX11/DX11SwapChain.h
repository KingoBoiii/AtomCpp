#pragma once
#include "Atom/Renderer/SwapChain.h"

#include <d3d11_4.h>

namespace Atom
{
	
	class DX11SwapChain : public SwapChain
	{
	public:
		DX11SwapChain(Window* window);
		virtual ~DX11SwapChain();

		virtual void Initialize() override;
		virtual void Present() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;
	private:
		void Invalidate();

		void CreateSwapChain(ID3D11Device* device);
		void CreateRenderTargetView(ID3D11Device* device);
		void CreateDepthStencilBufferAndView(ID3D11Device* device);
	private:
		Window* m_Window;

		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;

		friend class DX11Renderer;
	};

}
