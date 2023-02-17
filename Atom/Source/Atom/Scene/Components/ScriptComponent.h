#pragma once

namespace Atom::Component
{

	struct Script
	{
		std::string ClassName;

		Script() = default;
		Script(const Script&) = default;
		Script(const std::string& name) : ClassName(name) { }
	};

}