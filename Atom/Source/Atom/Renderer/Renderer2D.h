#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "Font.h"

namespace Atom
{

	
	class Renderer2D
	{
	public:
		struct Renderer2D::Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
	public:
		static void Initialize();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& cameraTransform);
		static void EndScene();

		// Draw calls
		
		static void RenderQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));

		static void RenderQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f);

		struct TextParameters
		{
			glm::vec4 Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};
		static void DrawString(const std::string& string, const Font* font, const glm::mat4& transform, const TextParameters& textParameters);

		static void ResetStats();
		static const Statistics& GetStats();
	private:
		static void CreateQuadPipeline();
		static void CreateCirclePipeline();
		static void CreateTextPipeline();

		static void NextBatch();
		static void StartBatch();
		static void Flush();
	};

}
