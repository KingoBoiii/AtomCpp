#pragma once
#include <glm/glm.hpp>

namespace Atom::Component
{

	struct CircleRenderer
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float Radius = 0.5f;
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRenderer() = default;
		CircleRenderer(const CircleRenderer&) = default;
	};

}