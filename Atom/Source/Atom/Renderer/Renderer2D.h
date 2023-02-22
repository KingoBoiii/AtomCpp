#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "Font.h"

namespace Atom
{
	
	class Renderer2D
	{
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

		static void DrawString(const std::string& string, const Font* font, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
	private:
		static void CreateQuadPipeline();
		static void CreateTextPipeline();

		static void NextBatch();
		static void StartBatch();
		static void Flush();
	};

}
