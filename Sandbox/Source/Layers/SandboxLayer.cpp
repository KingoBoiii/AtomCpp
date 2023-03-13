#include "SandboxLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void SandboxLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();
	
	Atom::ShaderOptions shaderOptions{ };
	shaderOptions.Filepath = "Assets/Shaders/BasicCamera.shader";
	shaderOptions.VertexShaderEntryPoint = "VSMain";
	shaderOptions.VertexShaderTarget = "vs_5_0";
	shaderOptions.PixelShaderEntryPoint = "PSMain";
	shaderOptions.PixelShaderTarget = "ps_5_0";
	Atom::Shader* shader = Atom::Shader::Create(shaderOptions);

	Atom::PipelineSpecification pipelineOptions{ };
	pipelineOptions.Shader = shader;
	pipelineOptions.InputLayout = {
		{ Atom::ShaderDataType::Float3, "POSITION" }
	};
	m_Pipeline = Atom::Pipeline::Create(pipelineOptions);

	Atom::VertexBufferSpecification vertexBufferOptions{ };
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
	m_VertexBuffer = Atom::VertexBuffer::Create(vertexBufferOptions);

	uint32_t* indices = new uint32_t[6]{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	m_IndexBuffer = Atom::IndexBuffer::Create(indices, 6);

	Atom::Camera camera = Atom::Camera({ 0.0f, 0.0f, 0.5f });

	struct Data
	{
		glm::mat4 ProjectionViewMatrix;
	};

	Data data{};
	
	float aspectRatio = window->GetAspectRatio();
	data.ProjectionViewMatrix = glm::perspective(90.0f, aspectRatio, 0.001f, 1000.0f) * camera.GetViewMatrix();

	m_UniformBuffer = Atom::UniformBuffer::Create(&data, sizeof(Data));
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(float deltaTime)
{
	Atom::Renderer::Clear();

	Atom::Renderer::RenderGeometry(m_Pipeline, m_VertexBuffer, m_IndexBuffer, m_UniformBuffer);
}
