#pragma once
#include <Atom.h>

#include "Panels/Viewport.h"
#include <Atom/Editor/EditorCamera.h>
#include <Atom/Editor/SceneHierarchyPanel.h>

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
	private: // Scene commands
		void NewScene();
		void OpenScene();
		void SaveAs();

		void OnScenePlay();
		void OnSceneStop();

		void OnDuplicateEntity();
	private: // Events
		bool OnKeyPressed(Atom::KeyPressedEvent& e);
	private:
		Scene* m_ActiveScene = nullptr;
		Scene* m_EditorScene = nullptr;

		Framebuffer* m_Framebuffer = nullptr;
		EditorCamera m_EditorCamera;

		// Panels
		Viewport* m_Viewport = nullptr;
		SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;

		enum class SceneState
		{
			Edit = 0,
			Play = 1
		};
		
		SceneState m_SceneState = SceneState::Edit;
	};

}
