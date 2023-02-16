#pragma once
#include "SwapChain.h"

#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Atom
{

	struct ATOM_API RendererOptions
	{
		float* ClearColor;
		SwapChain* SwapChain;
	};

	class ATOM_API Renderer
	{
	public:
		Renderer(const RendererOptions& rendererOptions);
		virtual ~Renderer() = default;

		virtual void Initialize() = 0;

		virtual void Clear() const = 0;

		virtual void RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, int indexCount = 0) const = 0;
	protected:
		RendererOptions m_RendererOptions;
	};

	class ATOM_API RendererFactory
	{
	public:
		static Renderer* Create(const RendererOptions& rendererOptions);
	};

}
