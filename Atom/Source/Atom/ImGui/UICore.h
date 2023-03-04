#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Atom::UI
{

	const char* GenerateID();
	const char* GenerateLabelID(std::string_view label);
	void PushID();
	void PopID();
	
	// Centered Popup panel (no resizing, static width and height, no moving)
	bool BeginStaticModal(const std::string& title, const glm::vec2& size, const glm::vec2& center);
	void EndStaticModal();

	void BeginPropertyGrid(uint32_t columns = 2);
	void EndPropertyGrid();

}
