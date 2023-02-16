#include "SandboxLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void SandboxLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();

	Atom::RendererOptions rendererOptions{ };
	rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
	rendererOptions.SwapChain = window->GetSwapChain();
	m_Renderer = Atom::RendererFactory::CreateRenderer(rendererOptions);
	m_Renderer->Initialize();

	Atom::ShaderOptions shaderOptions{ };
	shaderOptions.Filepath = "Assets/Shaders/BasicCamera.shader";
	shaderOptions.VertexShaderEntryPoint = "VSMain";
	shaderOptions.VertexShaderTarget = "vs_5_0";
	shaderOptions.PixelShaderEntryPoint = "PSMain";
	shaderOptions.PixelShaderTarget = "ps_5_0";
	Atom::Shader* shader = Atom::ShaderFactory::Create(shaderOptions);

	Atom::PipelineOptions pipelineOptions{ };
	pipelineOptions.Shader = shader;
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

	Atom::Camera camera = Atom::Camera({ 0.0f, 0.0f, 0.5f });

	struct Data
	{
		glm::mat4 ProjectionViewMatrix;
	};

	Data data{};
	
	float aspectRatio = window->GetAspectRatio();
	data.ProjectionViewMatrix = glm::perspective(90.0f, aspectRatio, 0.001f, 1000.0f) * camera.GetViewMatrix();

	m_UniformBuffer = Atom::UniformBufferFactory::Create(&data, sizeof(Data));
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(float deltaTime)
{
	m_Renderer->Clear();

	m_Renderer->RenderGeometry(m_Pipeline, m_VertexBuffer, m_IndexBuffer, m_UniformBuffer);
}
