#pragma once
#include <Atom.h>
#include <Atom/Editor/EditorCamera.h>

#include "Panels/Viewport.h"
#include "Panels/ProjectExplorer.h"
#include <Atom/Editor/EditorPanels.h>

struct ImGuiPayload;

namespace Atom
{
	
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(std::string_view projectPath) : Layer("Atom Editor Layer"), m_ProjectPath(projectPath)
		{
		}

		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnGUI() override;
		virtual void OnEvent(Atom::Event& e) override;
	private:
		void DrawTopMenuBar();
		void UI_Toolbar();

		void UI_ShowNewProjectDialog();

		void OnViewportDragDropTarget(const ImGuiPayload* payload);
	private: // Project commands
		void NewProject();
		void CreateProject(const std::filesystem::path& projectPath);
		void OpenProject();
		void OpenProject(const std::filesystem::path& filepath);
		void SaveProject();
		void SaveProjectAs();
		void SaveProject(const std::filesystem::path& filepath);
		void CloseProject(bool unloadProject = true);
	private: // Scene commands
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();
		void OnSceneSimulate();
		void OnScenePause();

		void OnDuplicateEntity();
	private: // Events
		bool OnKeyPressed(Atom::KeyPressedEvent& e);
		bool OnMouseButtonPressed(Atom::MouseButtonPressedEvent& e);
	private:
		std::string m_ProjectPath;
		std::filesystem::path m_EditorScenePath;
		Scene* m_ActiveScene = nullptr;
		Scene* m_EditorScene = nullptr;

		bool m_ShowNewProjectDialog = false;

		Framebuffer* m_Framebuffer = nullptr;
		EditorCamera m_EditorCamera;

		Entity m_HoveredEntity = { };
		
		// Panels
		Viewport* m_Viewport = nullptr;
		SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;
		StatisticsPanel* m_StatisticsPanel = nullptr;
		ScriptEngineInspectorPanel* m_ScriptEngineInspectorPanel = nullptr;
		ProjectExplorer* m_ProjectExplorer = nullptr;
		ProjectSettingsPanel* m_ProjectSettingsPanel = nullptr;

		enum class SceneState
		{
			Edit = 0,
			Play = 1,
			Simulate = 2
		};
		
		SceneState m_SceneState = SceneState::Edit;
	};

}
