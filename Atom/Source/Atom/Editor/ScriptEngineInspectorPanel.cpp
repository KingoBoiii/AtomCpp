#include "ATPCH.h"
#include "ScriptEngineInspectorPanel.h"

#include "Atom/Scripting/ScriptEngine.h"

#include <imgui.h>

namespace Atom
{

	static std::vector<AssemblyMetadata> s_LoadedAssembliesMetadata;

	void ScriptEngineInspectorPanel::DrawUI(bool& isOpen)
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
		s_LoadedAssembliesMetadata.clear();

		AssemblyInfo* coreAssembly = ScriptEngine::GetCoreAssemblyInfo();
		s_LoadedAssembliesMetadata.push_back(coreAssembly->Metadata);

		for(const auto& referencedAssemblyMetadata : coreAssembly->ReferencedAssemblies)
		{
			s_LoadedAssembliesMetadata.push_back(referencedAssemblyMetadata);
		}

		AssemblyInfo* appAssembly = ScriptEngine::GetAppAssemblyInfo();
		if(appAssembly)
		{
			s_LoadedAssembliesMetadata.push_back(appAssembly->Metadata);

			for(const auto& referencedAssemblyMetadata : appAssembly->ReferencedAssemblies)
			{
				auto it = std::find_if(s_LoadedAssembliesMetadata.begin(), s_LoadedAssembliesMetadata.end(), [&referencedAssemblyMetadata](const AssemblyMetadata& other)
				{
					return referencedAssemblyMetadata == other;
				});

				if(it != s_LoadedAssembliesMetadata.end())
					continue;

				s_LoadedAssembliesMetadata.push_back(referencedAssemblyMetadata);
			}
		}
	}

}