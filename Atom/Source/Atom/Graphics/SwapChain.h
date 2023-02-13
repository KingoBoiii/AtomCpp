#pragma once
#include "Atom/Display/Window.h"

namespace Atom
{

	struct ATOM_API SwapChainOptions
	{
		uint32_t Width;
		uint32_t Height;
		Window* Window;
	};

	class ATOM_API SwapChain
	{
	public:
		SwapChain(const SwapChainOptions& swapChainOptions);
		virtual ~SwapChain() = default;

		virtual void Initialize() = 0;
		virtual void Present() const = 0;
	protected:
		SwapChainOptions m_SwapChainOptions;
	};

	class SwapChainFactory
	{
	public:
		static SwapChain* Create(const SwapChainOptions& swapChainOptions);
	};

}
