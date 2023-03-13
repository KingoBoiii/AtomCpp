#pragma once
#include "Atom/Renderer/Font/Font.h"

#include <string>

namespace Atom::Component
{

	struct TextRenderer
	{
		std::string Text = "";
		Font* FontAsset = Font::GetDefaultFont();
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		TextRenderer() = default;
		TextRenderer(const TextRenderer& text) = default;
	};

}
