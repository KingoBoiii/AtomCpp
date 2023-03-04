#include "ATPCH.h"
#include "ProjectSettingsPanel.h"

#include "Atom/ImGui/ImGuiUtillities.h"
#include <imgui.h>

namespace Atom
{

#define ATOM_SCENE_DRAG_DROP_TARGET_TYPE "PROJECT_EXPLORER_ITEM_SCENE"

	void ProjectSettingsPanel::DrawUI(bool& isOpen)
	{

		ImGui::Begin("Project Settings", &isOpen);

		ShowProjectSettings();
		ShowScriptingSettings();

		ImGui::End();
	}

	void ProjectSettingsPanel::OnProjectChanged(Project* project)
	{
	}

	void ProjectSettingsPanel::ShowProjectSettings()
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_DefaultOpen;

		bool open = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 6.0f, 6.0f });

		ImGui::PushID("ProjectSettingsPanel");
		open = ImGui::TreeNodeEx("##project_section", treeNodeFlags, "Project", false);
		ImGui::PopID();

		if(open)
		{
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 100);

			ImGui::Text("Project Name");
			ImGui::NextColumn();
			ImGui::Text(Project::GetActiveProject()->GetConfig().Name.c_str());
			ImGui::NextColumn();

			ImGui::Text("Asset Directory");
			ImGui::NextColumn();
			ImGui::Text(Project::GetActiveProject()->GetConfig().AssetDirectory.string().c_str());
			ImGui::NextColumn();

			ImGui::Text("Start Scene");
			ImGui::NextColumn();
			const std::string& startSceneName = Project::GetActiveProject()->GetConfig().StartScene.filename().string();
			if(const char* filepath = UI::DragDropTarget(startSceneName, ATOM_SCENE_DRAG_DROP_TARGET_TYPE))
			{
				AT_CORE_TRACE("Changed start up scene to {0}", filepath);
				Project::GetActiveProject()->GetConfig().StartScene = filepath;
			}
			ImGui::NextColumn();

			ImGui::Columns(1);

			ImGui::TreePop();
		}

		ImGui::PopStyleVar(2);
	}

	void ProjectSettingsPanel::ShowScriptingSettings()
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_FramePadding;

		bool open = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 6.0f, 6.0f });

		ImGui::PushID("ProjectSettingsPanel");
		open = ImGui::TreeNodeEx("##scripting_section", treeNodeFlags, "Scripting", false);
		ImGui::PopID();

		if(open)
		{
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, 100);

			ImGui::Text("Script Module Path");
			ImGui::NextColumn();
			ImGui::Text(Project::GetScriptModulePath().string().c_str());
			ImGui::NextColumn();

			ImGui::Columns(1);

			ImGui::TreePop();
		}

		ImGui::PopStyleVar(2);
	}

}