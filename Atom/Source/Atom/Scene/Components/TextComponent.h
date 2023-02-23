#pragma once

#include <string>

namespace Atom::Component
{

	struct Text
	{
		std::string TextString = "";
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Text() = default;
		Text(const Text& text) = default;
	};

}
