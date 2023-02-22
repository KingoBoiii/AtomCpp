#pragma once
#include <Atom.h>
#include <Atom/Editor/EditorCamera.h>

#include "Panels/Viewport.h"
#include <Atom/Editor/EditorPanels.h>

namespace Atom
{
	
	class EditorLayer : public Layer
	{
	public:
		EditorLayer() : Layer("Atom Editor Layer")
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
	private: // Project commands
		void NewProject();
		void OpenProject();
		void OpenProject(const std::filesystem::path& filepath);
		void SaveProject();
		void SaveProjectAs();
	private: // Scene commands
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveAs();

		void OnScenePlay();
		void OnSceneStop();
		void OnSceneSimulate();
		void OnScenePause();

		void OnDuplicateEntity();
	private: // Events
		bool OnKeyPressed(Atom::KeyPressedEvent& e);
	private:
		std::filesystem::path m_EditorScenePath;
		Scene* m_ActiveScene = nullptr;
		Scene* m_EditorScene = nullptr;

		Framebuffer* m_Framebuffer = nullptr;
		EditorCamera m_EditorCamera;

		// Panels
		Viewport* m_Viewport = nullptr;
		SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;
		StatisticsPanel* m_StatisticsPanel = nullptr;

		enum class SceneState
		{
			Edit = 0,
			Play = 1,
			Simulate = 2
		};
		
		SceneState m_SceneState = SceneState::Edit;
	};

}
