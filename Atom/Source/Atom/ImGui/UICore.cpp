#include "ATPCH.h"
#include <imgui.h>

#include "UICore.h"

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

	bool BeginStaticModal(const std::string& title, const glm::vec2& size, const glm::vec2& center)
	{
		ImGui::SetNextWindowPos({ center.x, center.y }, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y));

		static constexpr ImGuiWindowFlags staticModalFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;
		bool opened = ImGui::BeginPopupModal(title.c_str(), nullptr, staticModalFlags);
		if (opened)
		{
			PushID();
		}
		return opened;
	}

	void EndStaticModal()
	{
		PopID();
		ImGui::EndPopup();
	}

	void BeginPropertyGrid(uint32_t columns)
	{
		PushID();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		ImGui::Columns(columns);
	}

	void EndPropertyGrid()
	{
		ImGui::Columns(1);
		ImGui::PopStyleVar(2); // ItemSpacing, FramePadding
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 18.0f);
		PopID();
	}

}