#include "ATPCH.h"
#include "Renderer2D.h"
#include "Renderer.h"

#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Shader.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Atom
{

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Renderer2DData
	{
		// TODO: RenderCaps

		static constexpr uint32_t MaxQuads = 20000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32;

		Pipeline* QuadPipeline;
		VertexBuffer* QuadVertexBuffer;
		IndexBuffer* QuadIndexBuffer;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];

		struct CameraDataCS
		{
			glm::mat4 ProjectionViewMatrix;
		};
		CameraDataCS CameraData;
		UniformBuffer* CameraUniformBuffer;
	};

	static Renderer2DData s_Renderer2DData;

	Renderer2D::Renderer2D(Renderer* renderer)
		: m_Renderer(renderer)
	{
		s_Renderer2DData = {};
	}

	void Renderer2D::Initialize()
	{
		Atom::ShaderOptions shaderOptions{ };
		shaderOptions.Filepath = "Assets/Shaders/Renderer2D.shader";
		shaderOptions.VertexShaderEntryPoint = "VSMain";
		shaderOptions.VertexShaderTarget = "vs_5_0";
		shaderOptions.PixelShaderEntryPoint = "PSMain";
		shaderOptions.PixelShaderTarget = "ps_5_0";
		Shader* shader = Atom::ShaderFactory::Create(shaderOptions);

		PipelineOptions pipelineOptions{ };
		pipelineOptions.InputLayout = {
			{ ShaderDataType::Float3, "POSITION" },
			{ ShaderDataType::Float4, "COLOR" }
		};
		pipelineOptions.Shader = shader;
		s_Renderer2DData.QuadPipeline = PipelineFactory::Create(pipelineOptions);

		s_Renderer2DData.QuadVertexBufferBase = new QuadVertex[s_Renderer2DData.MaxVertices];

		VertexBufferOptions vertexBufferOptions{ };
		vertexBufferOptions.Size = s_Renderer2DData.MaxVertices;
		vertexBufferOptions.Stride = pipelineOptions.InputLayout.GetStride();
		s_Renderer2DData.QuadVertexBuffer = VertexBufferFactory::Create(vertexBufferOptions);

		uint32_t* quadIndices = new uint32_t[s_Renderer2DData.MaxIndices];

		uint32_t offset = 0;
		for(uint32_t i = 0; i < s_Renderer2DData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		IndexBufferOptions indexBufferOptions{ };
		indexBufferOptions.Indices = quadIndices;
		indexBufferOptions.Count = s_Renderer2DData.MaxIndices;
		s_Renderer2DData.QuadIndexBuffer = IndexBufferFactory::Create(indexBufferOptions);

		s_Renderer2DData.CameraUniformBuffer = UniformBufferFactory::Create(&s_Renderer2DData.CameraData, sizeof(Renderer2DData::CameraDataCS));

		s_Renderer2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Renderer2DData.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Renderer2DData.CameraData.ProjectionViewMatrix = glm::perspective(90.0f, 1280.0f / 720.0f, 0.001f, 1000.0f) * camera.GetViewMatrix();
		s_Renderer2DData.CameraUniformBuffer->SetData(&s_Renderer2DData.CameraData, sizeof(Renderer2DData::CameraDataCS));

		s_Renderer2DData.QuadIndexCount = 0;
		s_Renderer2DData.QuadVertexBufferPtr = s_Renderer2DData.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::RenderQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		RenderQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		RenderQuad(transform, color);
	}

	void Renderer2D::RenderQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		constexpr size_t quadVertexCount = 4;

		if(s_Renderer2DData.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			//NextBatch();
		}

		for(size_t i = 0; i < quadVertexCount; i++)
		{
			s_Renderer2DData.QuadVertexBufferPtr->Position = transform * s_Renderer2DData.QuadVertexPositions[i];
			s_Renderer2DData.QuadVertexBufferPtr->Color = color;
			s_Renderer2DData.QuadVertexBufferPtr++;
		}

		s_Renderer2DData.QuadIndexCount += 6;
	}

	void Renderer2D::Flush()
	{
		if(s_Renderer2DData.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Renderer2DData.QuadVertexBufferPtr - (uint8_t*)s_Renderer2DData.QuadVertexBufferBase);
			s_Renderer2DData.QuadVertexBuffer->SetData(s_Renderer2DData.QuadVertexBufferBase, dataSize);
			
			m_Renderer->RenderGeometry(s_Renderer2DData.QuadPipeline, s_Renderer2DData.QuadVertexBuffer, s_Renderer2DData.QuadIndexBuffer, s_Renderer2DData.CameraUniformBuffer, s_Renderer2DData.QuadIndexCount);
		}
	}

}