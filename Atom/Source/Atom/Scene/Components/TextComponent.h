#pragma once
#include "Atom/Renderer/Font.h"

#include <string>

namespace Atom::Component
{

	struct Text
	{
		std::string TextString = "";
		Font* FontAsset = Font::GetDefaultFont();
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Text() = default;
		Text(const Text& text) = default;
	};

}
