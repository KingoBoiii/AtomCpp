#include "Viewport.h"

#include <Atom.h>
#include <Atom/Math/Math.h>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include "ImGuizmo.h"

namespace Atom
{

	Viewport::Viewport(Framebuffer* framebuffer, EditorCamera* editorCamera, SceneHierarchyPanel* sceneHierarchyPanel)
		: m_Framebuffer(framebuffer), m_EditorCamera(editorCamera), m_SceneHierarchyPanel(sceneHierarchyPanel)
	{
	}

	void Viewport::OnImGuiRender(bool& isOpen)
	{
		static bool opened = true;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", &opened, ImGuiWindowFlags_NoDecoration);

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		ImGui::Image(m_Framebuffer->GetImage(), viewportPanelSize);

		if(ImGui::BeginDragDropTarget())
		{
			m_DragDropCallback(ImGui::AcceptDragDropPayload("PROJECT_EXPLORER_ITEM_SCENE"));
			//if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			//{
			//	const wchar_t* path = (const wchar_t*)payload->Data;
			//	AT_CORE_ASSERT(false);
			//	//OpenScene(path);
			//}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Atom::Entity selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
		if(selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			//ImGuizmo::SetRect(m_Viewport->m_ViewportWindowPosition.x, m_Viewport->m_ViewportWindowPosition.y, m_Viewport->m_ViewportWindowSize.x, m_Viewport->m_ViewportWindowSize.y); // Set viewport

#if 0
			auto cameraEntity = m_Scene->GetPrimaryCameraEntity();
			auto& camera = cameraEntity.GetComponent<Component::Camera>().SceneCamera;

			const glm::mat4& cameraProjectionMatrix = camera.GetProjectionMatrix();
			glm::mat4 cameraViewMatrix = glm::inverse(cameraEntity.GetComponent<Component::Transform>().GetTransform());
#endif

			const glm::mat4& cameraProjection = m_EditorCamera->GetProjectionMatrix();
			const glm::mat4& cameraView = m_EditorCamera->GetViewMatrix();

			// Entity transform
			auto& transformComponent = selectedEntity.GetComponent<Component::Transform>();
			glm::mat4& transform = transformComponent.GetTransform();

			// Snapping
			bool snap = Atom::Input::IsKeyDown(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for position/scale
			// Snap to 45 degrees for rotation
			if(m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}

			float snapValues[3] = { snapValue, snapValue, snapValue };
			
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if(ImGuizmo::IsUsing())
			{
				glm::vec3 position, rotation, scale;
				Atom::Math::DecomposeTransform(transform, position, rotation, scale);

				glm::vec3 deltaRotation = rotation - transformComponent.Rotation;
				transformComponent.Position = position;
				transformComponent.Rotation += deltaRotation;
				transformComponent.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Viewport::SetSceneContext(Scene* scene)
	{
		m_Scene = scene;
	}

}