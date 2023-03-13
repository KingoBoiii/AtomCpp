#include "ATPCH.h"
#include "UICore.h"

#include <imgui.h>

namespace Atom::UI
{

	static int s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16] = "##";
	static char s_LabelIDBuffer[1024];

	const char* GenerateID()
	{
		_itoa_s(s_Counter++, s_IDBuffer + 2, sizeof(s_IDBuffer) - 2, 16);
		return s_IDBuffer;
	}

	const char* GenerateLabelID(std::string_view label)
	{
		*fmt::format_to_n(s_LabelIDBuffer, std::size(s_LabelIDBuffer), "{}##{}", label, s_Counter++).out = 0;
		return s_LabelIDBuffer;
	}

	void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}

	void BeginPropertyGrid(uint32_t columns, bool border)
	{
		PushID();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		ImGui::Columns(columns, (const char*)0, border);
	}

	void EndPropertyGrid()
	{
		ImGui::Columns(1);
		ImGui::PopStyleVar(2); // ItemSpacing, FramePadding
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 18.0f);
		PopID();
	}

	void PropertyGridText(const std::string& label, const std::string& value)
	{
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::Text("%s", value.c_str());
		ImGui::NextColumn();
	}

}