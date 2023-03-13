#pragma once

#include <string>

namespace Atom::UI
{

	const char* GenerateID();
	const char* GenerateLabelID(std::string_view label);
	void PushID();
	void PopID();
	
	void BeginPropertyGrid(uint32_t columns = 2, bool border = false);
	void EndPropertyGrid();

	void PropertyGridText(const std::string& label, const std::string& value);

}
