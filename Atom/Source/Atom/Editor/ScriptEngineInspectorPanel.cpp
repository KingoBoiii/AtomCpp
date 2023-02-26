#include "ATPCH.h"
#include "ScriptEngineInspectorPanel.h"

#include "Atom/Scripting/ScriptEngine.h"

#include <imgui.h>

namespace Atom
{

	static std::vector<AssemblyMetadata> s_LoadedAssembliesMetadata;

	void ScriptEngineInspectorPanel::OnImGuiRender(bool& isOpen)
	{
		ImGui::Begin("Script Engine Inspector", &isOpen);

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_DefaultOpen;

		bool open = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 6.0f, 6.0f });

		ImGui::PushID("");
		open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, fmt::format("Loaded Assemblies ({0})", s_LoadedAssembliesMetadata.size()).c_str(), false);
		ImGui::PopID();

		if(open)
		{
			for(const auto& referencedAssembly : s_LoadedAssembliesMetadata)
			{
				ImGui::Text("%s (%d.%d.%d.%d)", referencedAssembly.Name.c_str(), referencedAssembly.MajorVersion, referencedAssembly.MinorVersion, referencedAssembly.BuildVersion, referencedAssembly.RevisionVersion);
			}

			ImGui::TreePop();
		}

		ImGui::PopStyleVar(2);

		ImGui::End();
	}

	void ScriptEngineInspectorPanel::OnProjectChanged(Project* project)
	{
#if 0
		s_LoadedAssembliesMetadata.clear();

		AssemblyInfo* coreAssembly = ScriptEngine::GetCoreAssemblyInfo();
		s_LoadedAssembliesMetadata.push_back(coreAssembly->Metadata);

		AssemblyInfo* appAssembly = ScriptEngine::GetAppAssemblyInfo();
		s_LoadedAssembliesMetadata.push_back(appAssembly->Metadata);
#endif
	}

}