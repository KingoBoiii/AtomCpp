#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();

	Atom::RendererOptions rendererOptions{ };
	rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
	rendererOptions.SwapChain = window->GetSwapChain();
	m_Renderer = Atom::RendererFactory::Create(rendererOptions);
	m_Renderer->Initialize();

	Atom::ShaderOptions shaderOptions{ };
	shaderOptions.Filepath = "Assets/Shaders/Basic.shader";
	shaderOptions.VertexShaderEntryPoint = "VSMain";
	shaderOptions.VertexShaderTarget = "vs_5_0";
	shaderOptions.PixelShaderEntryPoint = "PSMain";
	shaderOptions.PixelShaderTarget = "ps_5_0";
	m_Shader = Atom::ShaderFactory::Create(shaderOptions);

	Atom::PipelineOptions pipelineOptions{ };
	pipelineOptions.Shader = m_Shader;
	pipelineOptions.InputLayout = {
		{ Atom::ShaderDataType::Float3, "POSITION" }
	};
	m_Pipeline = Atom::PipelineFactory::Create(pipelineOptions);

	Atom::VertexBufferOptions vertexBufferOptions{ };
	vertexBufferOptions.Vertices = new float[4 * 3]
	{
		0.5f, 0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f, 0.5f, 0.0f   // top left 
	};
	vertexBufferOptions.Size = 4 * 3 * sizeof(float);
	vertexBufferOptions.Stride = pipelineOptions.InputLayout.GetStride(); // 3 * sizeof(float);
	vertexBufferOptions.Offset = 0;
	m_VertexBuffer = Atom::VertexBufferFactory::Create(vertexBufferOptions);

	Atom::IndexBufferOptions indexBufferOptions{ };
	indexBufferOptions.Indices = new uint32_t[6]{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	indexBufferOptions.Count = 6;
	m_IndexBuffer = Atom::IndexBufferFactory::Create(indexBufferOptions);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate()
{
	m_Renderer->Clear();

	m_Renderer->RenderGeometry(m_Pipeline, m_VertexBuffer, m_IndexBuffer);
}
