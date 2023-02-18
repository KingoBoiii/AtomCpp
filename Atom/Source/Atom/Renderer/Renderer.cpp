#include "ATPCH.h"
#include "Renderer.h"
#include "IRenderer.h"
#include "Renderer2D.h"

#include "Atom/Platform/DirectX11/DX11Renderer.h"

namespace Atom
{

	static IRenderer* CreateRenderer(Window* window)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Renderer(window);
			case RendererAPI::OpenGL:		
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	static IRenderer* s_Renderer = nullptr;

	void Renderer::Initialize(Window* window)
	{
		s_Renderer = CreateRenderer(window);
		s_Renderer->Initialize();

		Renderer2D::Initialize();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();

		s_Renderer->Shutdown();
	}

	void Renderer::BeginFrame()
	{
		s_Renderer->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		s_Renderer->EndFrame();
	}

	void Renderer::Clear()
	{
		s_Renderer->Clear();
	}

	void Renderer::RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t indexCount)
	{
		s_Renderer->RenderGeometry(pipeline, vertexBuffer, indexBuffer, indexCount);
	}

	void Renderer::RenderGeometry(Pipeline* pipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, uint32_t indexCount)
	{
		s_Renderer->RenderGeometry(pipeline, vertexBuffer, indexBuffer, uniformBuffer, indexCount);
	}

	void Renderer::SetRendererAPI(RendererAPI rendererAPI)
	{
		AT_CORE_ASSERT(rendererAPI == RendererAPI::DirectX11, "DirectX 11 is currently the only supported Renderer API!");
		s_CurrentRendererAPI = rendererAPI;
	}

}