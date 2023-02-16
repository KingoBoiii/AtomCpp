#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();

	Atom::RendererOptions rendererOptions{ };
	rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
	rendererOptions.SwapChain = window->GetSwapChain();
	m_Renderer = Atom::RendererFactory::Create(rendererOptions);
	m_Renderer->Initialize();

	Atom::VertexBufferOptions vertexBufferOptions{ };
	vertexBufferOptions.Vertices = new float[3 * 3]
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	vertexBufferOptions.Size = 3 * 3 * sizeof(float);
	vertexBufferOptions.Stride = 3 * sizeof(float);
	vertexBufferOptions.Offset = 0;
	m_VertexBuffer = Atom::VertexBufferFactory::Create(vertexBufferOptions);

	Atom::IndexBufferOptions indexBufferOptions{ };
	indexBufferOptions.Indices = new uint32_t[3]{ 0, 1, 2 };
	indexBufferOptions.Count = 3;
	m_IndexBuffer = Atom::IndexBufferFactory::Create(indexBufferOptions);

	Atom::ShaderOptions shaderOptions{ };
	shaderOptions.Filepath = "Assets/Shaders/Basic.shader";
	shaderOptions.VertexShaderEntryPoint = "VSMain";
	shaderOptions.VertexShaderTarget = "vs_5_0";
	shaderOptions.PixelShaderEntryPoint = "PSMain";
	shaderOptions.PixelShaderTarget = "ps_5_0";
	m_Shader = Atom::ShaderFactory::Create(shaderOptions);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate()
{
	m_Renderer->Clear();

	m_Renderer->RenderGeometry(m_VertexBuffer, m_IndexBuffer);
}
