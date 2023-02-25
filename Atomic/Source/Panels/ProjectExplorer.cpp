#include "ProjectExplorer.h"
#include <Atom/Editor/EditorResources.h>

#include <Atom/Renderer/Texture2D.h>

#include <imgui.h>

namespace Atom
{

#define ATOM_SCENE_FILE_EXTENSION ".atsc"
#define ATOM_SCENE_DRAG_DROP_TARGET_TYPE "PROJECT_EXPLORER_ITEM_SCENE"

	using AtomFileExtension = std::string;
	using AtomFileExtensionMap = std::unordered_map<AtomFileExtension, std::string>;
	static AtomFileExtensionMap s_FileExtensionDragDropTargetMap = {
		{ ATOM_SCENE_FILE_EXTENSION, ATOM_SCENE_DRAG_DROP_TARGET_TYPE }
	};

	ProjectExplorer::ProjectExplorer()
		: m_BaseDirectory(""), m_CurrentDirectory(m_BaseDirectory)
	{
	}

	void ProjectExplorer::OnImGuiRender(bool& isOpen)
	{
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		ImGuiWindowFlags projectExplorerFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse; //ImGuiWindowFlags_None; // 
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Project Explorer", &isOpen, projectExplorerFlags);
		ImGui::PopStyleVar();

		ImGui::Columns(2, nullptr, false);

		ImGui::SetColumnWidth(0, 200);

		DrawAssetDirectoryTree();

#if 0
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);
#endif

		ImGui::NextColumn();

		float availableHeight = ImGui::GetContentRegionAvail().y - 29.0f;

		ImGui::BeginChild(m_CurrentDirectory.string().c_str(), ImVec2(0, availableHeight), false);
		{
			if(ImGui::BeginPopupContextWindow(0, 1))
			{
				if(ImGui::MenuItem("Open in Explorer"))
				{
					system(fmt::format("explorer {0}", m_CurrentDirectory.string()).c_str());
				}

				ImGui::EndPopup();
			}

			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (int)(panelWidth / cellSize);
			if(columnCount < 1)
				columnCount = 1;

			ImGui::Columns(columnCount, 0, false);

			for(auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				const auto& path = directoryEntry.path();
				std::string filenameString = path.filename().string();

				ImGui::PushID(filenameString.c_str());
				Texture2D* icon = directoryEntry.is_directory() ? EditorResources::DirectoryIcon : EditorResources::FileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)icon->GetTexture(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				if(ImGui::BeginDragDropSource())
				{
					std::filesystem::path relativePath(path);
					const std::string& itemPath = relativePath.string();

					size_t pos = itemPath.find("\\Scenes");
					const std::string& relativePathString = itemPath.substr(pos + 1);

					const std::string& extension = relativePath.extension().string();
					const std::string& dragDropTargetType = s_FileExtensionDragDropTargetMap.at(relativePath.extension().string());
					static_assert(sizeof(dragDropTargetType) > 32 + 1);

					ImGui::SetDragDropPayload(dragDropTargetType.c_str(), relativePathString.c_str(), (strlen(relativePathString.c_str()) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleColor();
				if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if(directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();

				}
				ImGui::TextWrapped(filenameString.c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::Columns(1);

		// Status bar
		ImGuiWindowFlags statusBarFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 2.0f));
		ImGui::BeginChild("Project_Explorer_Statusbar", ImVec2(0, 35), false, statusBarFlags);

		ImGui::Text("Base Directory: %s", m_BaseDirectory.string().c_str());
		ImGui::SameLine();
		ImGui::Text("Current Directory: %s", m_CurrentDirectory.string().c_str());

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void ProjectExplorer::OnProjectChanged(Project* project)
	{
		m_BaseDirectory = Project::GetAssetDirectory();
		m_CurrentDirectory = m_BaseDirectory;
	}

	void ProjectExplorer::DrawAssetDirectoryTree()
	{
		ImGuiTreeNodeFlags treeNodeFlags = ((m_CurrentDirectory == m_BaseDirectory) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding;
		bool opened = ImGui::TreeNodeEx("Assets", treeNodeFlags);
		if(ImGui::IsItemClicked())
		{
			m_CurrentDirectory = m_BaseDirectory;
		}

		if(opened)
		{
			DrawDirectoryTree(m_BaseDirectory);

			ImGui::TreePop();
		}
	}

	void ProjectExplorer::DrawDirectoryTree(const std::filesystem::path& directoryPath)
	{
		for(auto& directoryEntry : std::filesystem::directory_iterator(directoryPath))
		{
			if(!directoryEntry.is_directory())
			{
				continue;
			}

			const auto& path = directoryEntry.path();

			ImGuiTreeNodeFlags childTreeNodeFlags = ((m_CurrentDirectory == path) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding;

			bool opened1 = ImGui::TreeNodeEx(path.filename().string().c_str(), childTreeNodeFlags);
			if(ImGui::IsItemClicked())
			{
				m_CurrentDirectory = directoryPath / path.filename();
			}

			if(opened1)
			{
				DrawDirectoryTree(path);
				ImGui::TreePop();
			}
		}
	}

}