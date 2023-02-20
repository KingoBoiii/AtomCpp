#include "ATPCH.h"
#include "ImGuiUtillities.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Atom::UI
{

	void Column2(const std::string& label, std::function<void()> content, float columnWidth, bool border)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, 0, border);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		content();

		ImGui::Columns(1);
		ImGui::PopID();
	}

}