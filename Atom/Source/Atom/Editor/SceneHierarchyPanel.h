#pragma once
#include "EditorPanel.h"

#include "Atom/Scene/Entity.h"

namespace Atom
{

	class SceneHierarchyPanel : public EditorPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Scene* scene);

		virtual void OnImGuiRender(bool& isOpen) override;
		virtual void SetSceneContext(Scene* scene) override;

		Entity GetSelectedEntity() const { return m_SelectedEntity; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		void DrawTransformComponent(Component::Transform& component);
		void DrawCameraComponent(Component::Camera& component);
		void DrawBasicRendererComponent(Component::BasicRenderer& component);
		void DrawCircleRendererComponent(Component::CircleRenderer& component);
		void DrawScriptComponent(Component::Script& component);
		void DrawRigidbody2DComponent(Component::Rigidbody2D& component);
		void DrawBoxCollider2DComponent(Component::BoxCollider2D& component);
		void DrawTextComponent(Component::Text& component);

		void DrawAddComponentPopup();
		template<typename Component>
		void DrawAddComponentPopupItem(const std::string& text);
#if 0
		{
			if(ImGui::MenuItem(text.c_str()))
			{
				m_SelectedEntity.AddComponent<Component>();
				ImGui::CloseCurrentPopup();
			}
		}
#endif
	private:
		Scene* m_Scene = nullptr;
		Entity m_SelectedEntity = {};
	};

}