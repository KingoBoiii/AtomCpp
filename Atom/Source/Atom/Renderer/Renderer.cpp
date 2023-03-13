#include "ATPCH.h"
#include "Renderer.h"
#include "IRenderer.h"
#include "Renderer2D.h"
#include "ShaderLibrary.h"

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

	struct RendererData
	{
		ShaderLibrary* ShaderLibrary = nullptr;
	};

	static IRenderer* s_Renderer = nullptr;
	static RendererData* s_RendererData = nullptr;

	void Renderer::Initialize(Window* window)
	{
		s_Renderer = CreateRenderer(window);
		s_Renderer->Initialize();

		s_RendererData = new RendererData();

		s_RendererData->ShaderLibrary = ShaderLibrary::Create();

		Renderer::GetShaderLibrary()->Load("Resources/Shaders/Renderer2D.shader");
		Renderer::GetShaderLibrary()->Load("Resources/Shaders/Renderer2D_Circle.shader");
		Renderer::GetShaderLibrary()->Load("Resources/Shaders/Renderer2D_Text.shader");
		
		Renderer2D::Initialize();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();

		s_Renderer->Shutdown();
		delete s_Renderer;
		s_Renderer = nullptr;
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

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		if(!s_Renderer)
		{
			return;
		}

		s_Renderer->SetViewport(x, y, width, height);
	}

	ShaderLibrary* Renderer::GetShaderLibrary()
	{
		return s_RendererData->ShaderLibrary;
	}

	void Renderer::SetRendererAPI(RendererAPI rendererAPI)
	{
		AT_CORE_ASSERT(rendererAPI == RendererAPI::DirectX11, "DirectX 11 is currently the only supported Renderer API!");
		s_CurrentRendererAPI = rendererAPI;
	}

}