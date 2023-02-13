#include "ATPCH.h"
#include "SwapChain.h"

#include "DirectX11/DX11SwapChain.h"

namespace Atom
{

	SwapChain::SwapChain(const SwapChainOptions& swapChainOptions)
		: m_SwapChainOptions(swapChainOptions)
	{
	}

	SwapChain* SwapChainFactory::Create(const SwapChainOptions& swapChainOptions)
	{
		return new DX11SwapChain(swapChainOptions);
	}

}