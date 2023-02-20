#include "ATPCH.h"
#include "SceneHierarchyPanel.h"
#include "Atom/Scene/Components.h"

#include "Atom/Scripting/ScriptEngine.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace Atom
{

#define ADD_COMPONENT_POP_UP_IDENTIFIER "AddComponent"
#define COMPONENT_SETTINGS_POP_UP_IDENTIFIER "ComponentSettings"

	namespace Utils
	{

		template<typename Component>
		static void DrawComponent(const std::string& name, Entity entity, std::function<void(Component&)> uiFunction)
		{
			if(!entity.HasComponent<Component>())
			{
				return;
			}

			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			auto& component = entity.GetComponent<Component>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if(ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup(COMPONENT_SETTINGS_POP_UP_IDENTIFIER);
			}

			bool removeComponent = false;
			if(ImGui::BeginPopup(COMPONENT_SETTINGS_POP_UP_IDENTIFIER))
			{
				if(ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if(open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if(removeComponent)
			{
				entity.RemoveComponent<Component>();
			}
		}

		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if(ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();
		}

	}

	SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
	{
		SetSceneContext(scene);
	}

	void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene Hierarchy", &isOpen);
		ImGui::PopStyleVar();

		if(m_Scene)
		{
			auto& view = m_Scene->GetAllEntitiesWith<Component::Identifier>();
			for(auto e : view)
			{
				Entity entity{ e, m_Scene };
				DrawEntityNode(entity);
			}

			if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_SelectedEntity = {};
			}

			// Right-click on blank space
			if(ImGui::BeginPopupContextWindow(0, 1))
			{
				if(ImGui::MenuItem("Create Empty Entity"))
				{
					m_Scene->CreateEntity("Empty Entity");
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Entity Inspector");

		if(m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSceneContext(Scene* scene)
	{
		m_Scene = scene;
		m_SelectedEntity = { };
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<Component::Identifier>().Name;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if(ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		bool entityDeleted = false;
		if(ImGui::BeginPopupContextItem())
		{
			if(ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}

		if(opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, name.c_str());
			if(opened)
			{
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if(entityDeleted)
		{
			m_Scene->DestroyEntity(entity);
			if(m_SelectedEntity == entity)
			{
				m_SelectedEntity = {};
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if(entity.HasComponent<Component::Identifier>())
		{
			auto& name = entity.GetComponent<Component::Identifier>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));
			if(ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if(ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup(ADD_COMPONENT_POP_UP_IDENTIFIER);
		}
		DrawAddComponentPopup();

		ImGui::PopItemWidth();

		Utils::DrawComponent<Component::Transform>("Transform", entity, [](auto& component)
		{
			Utils::DrawVec3Control("Translation", component.Position);
		glm::vec3 rotation = glm::degrees(component.Rotation);
		Utils::DrawVec3Control("Rotation", rotation);
		component.Rotation = glm::radians(rotation);
		Utils::DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		Utils::DrawComponent<Component::BasicRenderer>("Basic Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});

		Utils::DrawComponent<Component::Script>("C# Script", entity, [entity](auto& component) mutable
		{
			bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);

		static char buffer[64];
		strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

		if(!scriptClassExists)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));
		}

		if(ImGui::InputText("Class", buffer, sizeof(buffer)))
		{
			component.ClassName = buffer;
		}

		// Fields
		ScriptInstance* scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
		if(scriptInstance)
		{
			const auto& fields = scriptInstance->GetScriptClass()->GetFields();
			for(const auto& [name, field] : fields)
			{
				if(field.Type == ScriptFieldType::Float)
				{
					float data = scriptInstance->GetFieldValue<float>(name);
					if(ImGui::DragFloat(name.c_str(), &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
				}
			}
		}


		if(!scriptClassExists)
		{
			ImGui::PopStyleColor();
		}
		});
	}

	void SceneHierarchyPanel::DrawAddComponentPopup()
	{
		if(ImGui::BeginPopup(ADD_COMPONENT_POP_UP_IDENTIFIER))
		{
			DrawAddComponentPopupItem<Component::Camera>("Camera");
			DrawAddComponentPopupItem<Component::Script>("C# Script");
			DrawAddComponentPopupItem<Component::BasicRenderer>("Basic Renderer");

			ImGui::EndPopup();
		}
	}

	template<typename Component>
	inline void SceneHierarchyPanel::DrawAddComponentPopupItem(const std::string& text)
	{
		if(ImGui::MenuItem(text.c_str()))
		{
			m_SelectedEntity.AddComponent<Component>();
			ImGui::CloseCurrentPopup();
		}
	}

}