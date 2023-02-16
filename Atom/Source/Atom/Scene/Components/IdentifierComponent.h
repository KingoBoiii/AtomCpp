#pragma once
#include "Atom/Core/UUID.h"

#include <string>

namespace Atom::Component
{

	struct Identifier
	{
		std::string Name;
		UUID ID;

		Identifier() = default;
		Identifier(const Identifier&) = default;
		Identifier(const UUID& uuid) : ID(uuid) { }
	};

}
