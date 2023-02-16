#pragma once
#include "SwapChain.h"

namespace Atom
{

	struct ATOM_API RendererOptions
	{
		float* ClearColor;
		SwapChain* SwapChain;
	};

	class Renderer
	{
	public:
		Renderer(const RendererOptions& rendererOptions);
		virtual ~Renderer() = default;

		virtual void Initialize() = 0;

		virtual void Clear() const = 0;
	protected:
		RendererOptions m_RendererOptions;
	};

	class RendererFactory
	{
	public:
		static Renderer* Create(const RendererOptions& rendererOptions);
	};

}
