#pragma once

#include <string>

namespace Atom::Component
{

	struct Text
	{
		std::string TextString = "";

		Text() = default;
		Text(const Text& text) = default;
	};

}
