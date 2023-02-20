#pragma once

namespace Atom::UI
{

	void Column2(const std::string& label, std::function<void()> content, float columnWidth = 100.0f, bool border = false);

	// IMPORTANT: Remember to call ImGui::End() after calling this function!
	void Dockspace(const char* windowName = "DockSpace Demo", const char* id = "MyDockspace");

}
