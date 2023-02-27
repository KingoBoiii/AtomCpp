#include "ATPCH.h"
#include "Renderer2D.h"
#include "Renderer.h"

#include "Camera.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Shader.h"

#include "MSDFData.h"

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

	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DData
	{
		// TODO: RenderCaps

		static constexpr uint32_t MaxQuads = 200000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 16; // 32 is the max for OpenGL, 16 for DirectX

		Pipeline* QuadPipeline;
		VertexBuffer* QuadVertexBuffer;
		IndexBuffer* QuadIndexBuffer;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		Pipeline* TextPipeline;
		VertexBuffer* TextVertexBuffer;

		uint32_t TextIndexCount = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr = nullptr;

		Texture2D* FontAtlasTexture;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

		struct CameraDataCS
		{
			glm::mat4 ProjectionViewMatrix;
		};
		CameraDataCS CameraData;
		UniformBuffer* CameraUniformBuffer;
	};

	static Renderer2DData s_Renderer2DData;

	void Renderer2D::Initialize()
	{
		s_Renderer2DData = {};

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

		s_Renderer2DData.QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_Renderer2DData.MaxIndices);

		CreateQuadPipeline();
		CreateTextPipeline();

		s_Renderer2DData.CameraUniformBuffer = UniformBuffer::Create(&s_Renderer2DData.CameraData, sizeof(Renderer2DData::CameraDataCS));

		s_Renderer2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Renderer2DData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Renderer2DData.QuadVertexBufferBase;
		delete[] s_Renderer2DData.TextVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Renderer2DData.CameraData.ProjectionViewMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
		s_Renderer2DData.CameraUniformBuffer->SetData(&s_Renderer2DData.CameraData, sizeof(Renderer2DData::CameraDataCS));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		s_Renderer2DData.CameraData.ProjectionViewMatrix = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);
		s_Renderer2DData.CameraUniformBuffer->SetData(&s_Renderer2DData.CameraData, sizeof(Renderer2DData::CameraDataCS));

		StartBatch();
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
			NextBatch();
		}

		for(size_t i = 0; i < quadVertexCount; i++)
		{
			s_Renderer2DData.QuadVertexBufferPtr->Position = transform * s_Renderer2DData.QuadVertexPositions[i];
			s_Renderer2DData.QuadVertexBufferPtr->Color = color;
			s_Renderer2DData.QuadVertexBufferPtr++;
		}

		s_Renderer2DData.QuadIndexCount += 6;

		s_Renderer2DData.Stats.QuadCount++;
	}

	void Renderer2D::DrawString(const std::string& string, const Font* font, const glm::mat4& transform, const TextParameters& textParameters)
	{
		const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		auto fontAtlas = font->GetAtlasTexture();

		s_Renderer2DData.FontAtlasTexture = fontAtlas;

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0f; // -fsScale * metrics.ascenderY;

		//if(s_Renderer2DData.QuadIndexCount >= Renderer2DData::MaxIndices)
		//{
		//	NextBatch();
		//}

		float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

		for(size_t i = 0; i < string.size(); i++)
		{
			char character = string[i];

			if(character == '\n')
			{
				x = 0.0;
				y -= fsScale * metrics.lineHeight + textParameters.LineSpacing;
				continue;
			}

			if(character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if(i < string.size() - 1)
				{
					char nextCharacter = string[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}
				x += fsScale * advance + textParameters.Kerning;
				continue;
			}

			if(character == '\t')
			{
				x += 4.0f * (fsScale * spaceGlyphAdvance + textParameters.Kerning);
				continue;
			}

			auto glyph = fontGeometry.getGlyph(character);
			if(!glyph)
			{
				glyph = fontGeometry.getGlyph('?');
			}
			if(!glyph)
			{
				return;
			}

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			// Render here!
			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = textParameters.Color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = textParameters.Color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = textParameters.Color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = textParameters.Color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextIndexCount += 6;

			s_Renderer2DData.Stats.QuadCount++;

			if(i < string.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = string[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + textParameters.Kerning;
			}
		}
	}

	void Renderer2D::DrawString(const std::string& string, const Font* font, const glm::mat4& transform, const glm::vec4 color, float kerning, float lineSpacing)
	{
		const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		auto fontAtlas = font->GetAtlasTexture();

		s_Renderer2DData.FontAtlasTexture = fontAtlas;

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0f; // -fsScale * metrics.ascenderY;

		//if(s_Renderer2DData.QuadIndexCount >= Renderer2DData::MaxIndices)
		//{
		//	NextBatch();
		//}

		float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

		for(size_t i = 0; i < string.size(); i++)
		{
			char character = string[i];

			if(character == '\n')
			{
				x = 0.0;
				y -= fsScale * metrics.lineHeight + lineSpacing;
				continue;
			}

			if(character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if(i < string.size() - 1)
				{
					char nextCharacter = string[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}
				x += fsScale * advance + kerning;
				continue;
			}

			if(character == '\t')
			{
				x += 4.0f * (fsScale * spaceGlyphAdvance + kerning);
				continue;
			}

			auto glyph = fontGeometry.getGlyph(character);
			if(!glyph)
			{
				glyph = fontGeometry.getGlyph('?');
			}
			if(!glyph)
			{
				return;
			}

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			// Render here!
			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Renderer2DData.TextVertexBufferPtr->Color = color;
			s_Renderer2DData.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Renderer2DData.TextVertexBufferPtr++;

			s_Renderer2DData.TextIndexCount += 6;

			s_Renderer2DData.Stats.QuadCount++;

			if(i < string.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = string[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + kerning;
			}
		}
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Renderer2DData.Stats, 0, sizeof(Statistics));
	}

	const Renderer2D::Statistics& Renderer2D::GetStats()
	{
		return s_Renderer2DData.Stats;
	}

	void Renderer2D::CreateQuadPipeline()
	{
		PipelineOptions pipelineOptions{ };
		pipelineOptions.InputLayout = {
			{ ShaderDataType::Float3, "POSITION" },
			{ ShaderDataType::Float4, "COLOR" }
		};
		pipelineOptions.Shader = Renderer::GetShaderLibrary()->Get("Renderer2D");
		s_Renderer2DData.QuadPipeline = Pipeline::Create(pipelineOptions);

		s_Renderer2DData.QuadVertexBufferBase = new QuadVertex[s_Renderer2DData.MaxVertices];

		VertexBufferOptions vertexBufferOptions{ };
		vertexBufferOptions.Size = s_Renderer2DData.MaxVertices;
		vertexBufferOptions.Stride = pipelineOptions.InputLayout.GetStride();
		s_Renderer2DData.QuadVertexBuffer = VertexBuffer::Create(vertexBufferOptions);
	}

	void Renderer2D::CreateTextPipeline()
	{
		PipelineOptions pipelineOptions{ };
		pipelineOptions.InputLayout = {
			{ ShaderDataType::Float3, "POSITION" },
			{ ShaderDataType::Float4, "COLOR" },
			{ ShaderDataType::Float2, "TEXCOORD" }
		};
		pipelineOptions.Shader = Renderer::GetShaderLibrary()->Get("Renderer2D_Text");
		s_Renderer2DData.TextPipeline = Pipeline::Create(pipelineOptions);

		s_Renderer2DData.TextVertexBufferBase = new TextVertex[s_Renderer2DData.MaxVertices];

		VertexBufferOptions vertexBufferOptions{ };
		vertexBufferOptions.Size = s_Renderer2DData.MaxVertices;
		vertexBufferOptions.Stride = pipelineOptions.InputLayout.GetStride();
		s_Renderer2DData.TextVertexBuffer = VertexBuffer::Create(vertexBufferOptions);
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
		s_Renderer2DData.QuadIndexCount = 0;
		s_Renderer2DData.QuadVertexBufferPtr = s_Renderer2DData.QuadVertexBufferBase;

		s_Renderer2DData.TextIndexCount = 0;
		s_Renderer2DData.TextVertexBufferPtr = s_Renderer2DData.TextVertexBufferBase;

		ResetStats();
	}

	void Renderer2D::Flush()
	{
		if(s_Renderer2DData.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Renderer2DData.QuadVertexBufferPtr - (uint8_t*)s_Renderer2DData.QuadVertexBufferBase);
			s_Renderer2DData.QuadVertexBuffer->SetData(s_Renderer2DData.QuadVertexBufferBase, dataSize);

			Renderer::RenderGeometry(s_Renderer2DData.QuadPipeline, s_Renderer2DData.QuadVertexBuffer, s_Renderer2DData.QuadIndexBuffer, s_Renderer2DData.CameraUniformBuffer, s_Renderer2DData.QuadIndexCount);
			s_Renderer2DData.Stats.DrawCalls++;
		}

		if(s_Renderer2DData.TextIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Renderer2DData.TextVertexBufferPtr - (uint8_t*)s_Renderer2DData.TextVertexBufferBase);
			s_Renderer2DData.TextVertexBuffer->SetData(s_Renderer2DData.TextVertexBufferBase, dataSize);

			s_Renderer2DData.FontAtlasTexture->Bind();
			Renderer::RenderGeometry(s_Renderer2DData.TextPipeline, s_Renderer2DData.TextVertexBuffer, s_Renderer2DData.QuadIndexBuffer, s_Renderer2DData.CameraUniformBuffer, s_Renderer2DData.TextIndexCount);
			s_Renderer2DData.Stats.DrawCalls++;
		}
	}

}