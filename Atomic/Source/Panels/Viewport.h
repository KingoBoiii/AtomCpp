#pragma once
#include <Atom/Editor/EditorPanel.h>
#include <Atom/Editor/SceneHierarchyPanel.h>

#include <Atom/Renderer/Framebuffer.h>

struct ImGuiPayload;

namespace Atom
{

	class Viewport : public EditorPanel
	{
	public:
		Viewport() = default;
		Viewport(Framebuffer* framebuffer, EditorCamera* editorCamera, SceneHierarchyPanel* sceneHierarchyPanel);

		virtual void OnImGuiRender(bool& isOpen) override;
		virtual void SetSceneContext(Atom::Scene* scene) override;

		void SetDragDropCallback(std::function<void(const ImGuiPayload*)> callback) { m_DragDropCallback = callback; }

		void SetGizmoType(int type) { m_GizmoType = type; }
		const glm::vec2& GetViewportSize() const { return m_ViewportSize; }
	private:
		Framebuffer* m_Framebuffer = nullptr;
		EditorCamera* m_EditorCamera = nullptr;

		SceneHierarchyPanel* m_SceneHierarchyPanel = nullptr;
		Scene* m_Scene = nullptr;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		std::function<void(const ImGuiPayload*)> m_DragDropCallback;

		bool m_ViewportHovered = false;
		bool m_ViewportFocused = false;

		int m_GizmoType = -1;

		friend class EditorLayer;
	};

}
