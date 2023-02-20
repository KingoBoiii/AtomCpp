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
	private:
		void DrawDockSpace();
		void DrawTopMenuBar();
	private:
		Atom::Scene* m_Scene = nullptr;
		Atom::Framebuffer* m_Framebuffer = nullptr;

		// Panels
		Viewport* m_Viewport = nullptr;
		Atom::SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;
	};

}
