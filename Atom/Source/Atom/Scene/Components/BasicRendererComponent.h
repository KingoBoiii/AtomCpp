#pragma once
#include <glm/glm.hpp>

namespace Atom::Component
{

	struct BasicRenderer
	{
		glm::vec4 Color;

		BasicRenderer() = default;
		BasicRenderer(const BasicRenderer&) = default;
		BasicRenderer(const glm::vec4& color) : Color(color) { }
	};

}
