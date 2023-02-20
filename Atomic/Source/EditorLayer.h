#pragma once
#include <Atom.h>

#include "Panels/Viewport.h"
#include <Atom/Editor/SceneHierarchyPanel.h>

namespace Atomic
{
	
	class EditorLayer : public Atom::Layer
	{
	public:
		EditorLayer() : Layer("Atomic Editor Layer")
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
	private: // Scene commands
		void NewScene();
		void OpenScene();
		void SaveAs();
	private: // Events
		bool OnKeyPressed(Atom::KeyPressedEvent& e);
	private:
		Atom::Scene* m_Scene = nullptr;
		Atom::Framebuffer* m_Framebuffer = nullptr;

		// Panels
		Viewport* m_Viewport = nullptr;
		Atom::SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;
	};

}
