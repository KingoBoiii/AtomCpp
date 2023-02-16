#pragma once
#include "Atom/Graphics/SwapChain.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11SwapChain : public SwapChain
	{
	public:
		DX11SwapChain(const SwapChainOptions& swapChainOptions);
		virtual ~DX11SwapChain();

		virtual void Initialize() override;
		virtual void Present() const override;
	private:
		void CreateSwapChain();
	private:
		IDXGISwapChain* m_SwapChain = nullptr;

		friend class DX11Renderer;
	};

}
