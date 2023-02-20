#pragma once
#include <glm/glm.hpp>

namespace Atom::Component
{

	struct BasicRenderer
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		BasicRenderer() = default;
		BasicRenderer(const BasicRenderer&) = default;
		BasicRenderer(const glm::vec4& color) : Color(color) { }
	};

}
