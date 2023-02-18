#pragma once
#include "RendererAPI.h"
#include "Atom/Display/Window.h"

namespace Atom
{

	class Window;
	class Pipeline;
	class VertexBuffer;
	class IndexBuffer;
	class UniformBuffer;

	class Renderer
	{
	public:
		static void Initialize(Window* window);
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		static void Clear();

		static void RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t indexCount = 0);
		static void RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, uint32_t indexCount = 0);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		inline static RendererAPI Current() { return s_CurrentRendererAPI; }
		static void SetRendererAPI(RendererAPI rendererAPI);
	private:
		inline static RendererAPI s_CurrentRendererAPI = RendererAPI::DirectX11;
	};

};
