#include "NewProjectModal.h"

#include <Atom/ImGui/UICore.h>
#include <Atom/ImGui/ImGuiStyle.h>
#include <Atom/Utils/PlatformUtils.h>

#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>

namespace Atom
{

#define MAX_PROJECT_NAME_LENGTH 255
#define MAX_PROJECT_FILEPATH_LENGTH 512

	static char* s_ProjectNameBuffer = new char[MAX_PROJECT_NAME_LENGTH];
	static char* s_OpenProjectFilePathBuffer = new char[MAX_PROJECT_FILEPATH_LENGTH];
	static char* s_NewProjectFilePathBuffer = new char[MAX_PROJECT_FILEPATH_LENGTH];

	static ImFont* s_ProjectNameFont = nullptr;
	static ImFont* s_ProjectLocationFont = nullptr;

	NewProjectModal::NewProjectModal(const std::string& modalId, const std::function<void(const std::string&, const std::string&)> onCreateProjectCallback)
		: m_ModalId(modalId), m_OnCreateProjectCallback(onCreateProjectCallback)
	{
		memset(s_ProjectNameBuffer, 0, MAX_PROJECT_NAME_LENGTH);
		memset(s_OpenProjectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);
		memset(s_NewProjectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);

		ImGuiIO& io = ImGui::GetIO();

		s_ProjectNameFont = io.Fonts->AddFontFromFileTTF("Resources/Fonts/OpenSans/OpenSans-Bold.ttf", 36.0f);
		s_ProjectLocationFont = io.Fonts->AddFontFromFileTTF("Resources/Fonts/OpenSans/OpenSans-Bold.ttf", 20.00f);
	}

	NewProjectModal::~NewProjectModal()
	{
		s_ProjectNameFont = nullptr;
		s_ProjectLocationFont = nullptr;
	}

	void NewProjectModal::DrawUI()
	{
		if (!m_IsOpen)
		{
			return;
		}

		ImGui::OpenPopup(m_ModalId.c_str());

		ImVec2 mainViewportCenter = ImGui::GetMainViewport()->GetCenter();
		glm::vec2 center = { mainViewportCenter.x, mainViewportCenter.y };
		if (UI::BeginStaticModal(m_ModalId, { 600.0f, 0.0f }, center))
		{
			static float frameRounding = 5.5f;

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));

			// Project Name
			{
				UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
				UI::ScopedStyle frameRounding(ImGuiStyleVar_FrameRounding, frameRounding);

				UI::ScopedFont projectNameFont(s_ProjectNameFont);

				ImGui::SetNextItemWidth(-1);
				ImGui::InputTextWithHint("##new_project_name", "Project Name", s_ProjectNameBuffer, MAX_PROJECT_NAME_LENGTH);
			}

			// Project Location 
			{
				UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
				UI::ScopedStyle frameRounding(ImGuiStyleVar_FrameRounding, frameRounding);

				UI::ScopedFont projectLocationFont(s_ProjectLocationFont);

				ImVec2 label_size = ImGui::CalcTextSize("...", NULL, true);
				auto& style = ImGui::GetStyle();
				ImVec2 button_size = ImGui::CalcItemSize(ImVec2(0, 0), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

				ImGui::SetNextItemWidth(600.0f - button_size.x - style.FramePadding.x * 2.0f - style.ItemInnerSpacing.x - 1);
				ImGui::InputTextWithHint("##new_project_location", "Project Location", s_NewProjectFilePathBuffer, MAX_PROJECT_FILEPATH_LENGTH);

				ImGui::SameLine();

				if (ImGui::Button("..."))
				{
					std::filesystem::path result = FileDialogs::OpenFolderDialog();
					memcpy(s_NewProjectFilePathBuffer, result.string().data(), result.string().length());
				}
			}

			ImGui::PopStyleVar();

			std::string fullProjectPath = strlen(s_NewProjectFilePathBuffer) > 0 ? std::string(s_NewProjectFilePathBuffer) + "/" + std::string(s_ProjectNameBuffer) : "";
			ImGui::Text("Full Project Path: %s", fullProjectPath.c_str());

			ImGui::Separator();

			{
				UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
				UI::ScopedFont font(s_ProjectLocationFont);

				if (ImGui::Button("Create"))
				{
					//CreateProject(fullProjectPath);
					m_OnCreateProjectCallback(fullProjectPath, s_ProjectNameBuffer);
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					memset(s_NewProjectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);
					ImGui::CloseCurrentPopup();
					m_IsOpen = false;
				}
			}

			UI::EndStaticModal();
		}
	}

	void NewProjectModal::Open()
	{
		m_IsOpen = true;
	}

}