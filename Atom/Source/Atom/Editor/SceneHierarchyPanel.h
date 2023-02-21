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
		Scene* m_Scene;
		Entity m_SelectedEntity;
	};

}