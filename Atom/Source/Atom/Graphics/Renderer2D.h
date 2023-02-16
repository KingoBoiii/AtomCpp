#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

namespace Atom
{
	
	class ATOM_API Renderer;

	class ATOM_API Renderer2D
	{
	public:
		Renderer2D(Renderer* renderer);
		~Renderer2D() = default;

		void Initialize();
		void Shutdown();

		void BeginScene(const Camera& camera);
		void EndScene();

		// Draw calls
		
		void RenderQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		void RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));

		void RenderQuad(const glm::mat4& transform, const glm::vec4& color);
	private:
		void Flush();
	private:
		Renderer* m_Renderer;
	};

}
