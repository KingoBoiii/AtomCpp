#include "ATPCH.h"
#include "AssetManagerPanel.h"

#include "Atom/Asset/AssetManager.h"

#include <Atom/ImGui/UICore.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Atom
{

	void AssetManagerPanel::OnImGuiRender(bool& isOpen)
	{
		const auto& assetRegistry = AssetManager::GetAssetRegistry();

		ImGui::Begin("Asset Manager", &isOpen);

		for(const auto& [handle, metadata] : assetRegistry)
		{
			UI::BeginPropertyGrid();

			ImGui::SetColumnWidth(0, 100.0f);

			ImGui::Text("Handle");
			ImGui::NextColumn();
			ImGui::Text("%llu", metadata.Handle);
			ImGui::NextColumn();

			ImGui::Text("Type");
			ImGui::NextColumn();
			ImGui::Text("%s", Utils::AssetTypeToString(metadata.Type).c_str());
			ImGui::NextColumn();

			ImGui::Text("Filepath");
			ImGui::NextColumn();
			ImGui::Text("%s", metadata.Filepath.string().c_str());
			ImGui::NextColumn();

			UI::EndPropertyGrid();
		}

		ImGui::End();
	}

}