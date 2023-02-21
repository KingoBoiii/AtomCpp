#include "EditorLayer.h"
#include <Atom/Scene/SceneSerializer.h>
#include <Atom/Utils/PlatformUtils.h>
#include <Atom/ImGui/ImGuiUtillities.h>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include "ImGuizmo.h"

namespace Atom
{

#define ATOM_SCENE_FILE_EXTENSIONS "atsc"
#define ATOM_SCENE_FILE_DIALOG_FILTER "Atom Scene (*.atsc)\0*.atsc\0"

	void EditorLayer::OnAttach()
	{
		Window* window = Application::Get().GetWindow();

		m_ActiveScene = new Atom::Scene();

		Atom::FramebufferOptions framebufferOptions{ };
		framebufferOptions.ClearColor = new float[4] { 0.15f, 0.15f, 0.15f, 1.0f };
		framebufferOptions.Width = window->GetWidth();
		framebufferOptions.Height = window->GetHeight();
		m_Framebuffer = Framebuffer::Create(framebufferOptions);

		m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);

		m_SceneHierarchyPanel = new SceneHierarchyPanel(m_ActiveScene);
		m_Viewport = new Viewport(m_Framebuffer, &m_EditorCamera, m_SceneHierarchyPanel);
		m_Viewport->SetSceneContext(m_ActiveScene);


#if 0
		auto cameraEntity = m_Scene->CreateEntity("Camera");
		auto& transform = cameraEntity.GetComponent<Atom::Component::Transform>();
		transform.Position = { 0.0f, 0.0f, 0.1f };
		auto& camera = cameraEntity.AddComponent<Atom::Component::Camera>();

		auto quadEntity = m_Scene->CreateEntity("Quad");
		quadEntity.AddComponent<Atom::Component::BasicRenderer>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		quadEntity.AddComponent<Atom::Component::Script>("Sandbox.Player");
#endif

		//m_ActiveScene->OnRuntimeStart();
	}

	void EditorLayer::OnDetach()
	{
		//m_ActiveScene->OnRuntimeStop();
		delete m_ActiveScene;
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		FramebufferOptions framebufferOptions = m_Framebuffer->GetOptions();
		if(m_Viewport->GetViewportSize().x > 0.0f && m_Viewport->GetViewportSize().y > 0.0f && (framebufferOptions.Width != m_Viewport->GetViewportSize().x || framebufferOptions.Height != m_Viewport->GetViewportSize().y))
		{
			const glm::vec2& viewportSize = m_Viewport->GetViewportSize();
			m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_EditorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
		}

		m_EditorCamera.OnUpdate(deltaTime);

		m_Framebuffer->Bind();
		m_Framebuffer->Clear();

		m_ActiveScene->OnRuntimeEditor(deltaTime, m_EditorCamera);
		//m_ActiveScene->OnRuntimeUpdate(deltaTime);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnGUI()
	{
		Atom::UI::Dockspace();
		DrawTopMenuBar();

		static bool isOpen = true;
		m_Viewport->OnImGuiRender(isOpen);
		m_SceneHierarchyPanel->OnImGuiRender(isOpen);

#if 1
		static bool opened = true;
		ImGui::ShowDemoWindow(&opened);
#endif

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(AT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::DrawTopMenuBar()
	{
		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				if(ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveAs();
				}

				ImGui::Separator();

				std::string filepath = fmt::format("Assets/Scenes/Example.{}", ATOM_SCENE_FILE_EXTENSIONS);

				if(ImGui::MenuItem("Save Example Scene"))
				{
					SceneSerializer serializer(m_ActiveScene);
					serializer.Serialize(filepath);
				}
				if(ImGui::MenuItem("Load Example Scene"))
				{
					SceneSerializer serializer(m_ActiveScene);
					serializer.Deserialize(filepath);

					m_ActiveScene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Restart (W.I.P.)"))
				{
					Application::Get().Restart();
				}
				if(ImGui::MenuItem("Close"))
				{
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = new Scene();
		m_ActiveScene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
		m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);
		m_Viewport->SetSceneContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile(ATOM_SCENE_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			AT_CORE_WARN("Cancelled open file dialog!");
			return;
		}

		NewScene();

		SceneSerializer serializer(m_ActiveScene);
		auto d = serializer.Deserialize(filepath);

		m_ActiveScene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
	}

	void EditorLayer::SaveAs()
	{
		std::string filepath = FileDialogs::SaveFile(ATOM_SCENE_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			AT_CORE_WARN("Cancelled save file dialog!");
			return;
		}

		SceneSerializer serializer(m_ActiveScene);
		serializer.Serialize(filepath);
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if(e.GetRepeatCount() > 0)
		{
			return false;
		}

		bool control = Input::IsKeyDown(Key::LeftControl) || Atom::Input::IsKeyDown(Key::RightControl);
		bool shift = Input::IsKeyDown(Key::LeftShift) || Input::IsKeyDown(Key::RightShift);

		switch(e.GetKeyCode())
		{
			// Gizmo shortcuts
			case Key::Q:
				m_Viewport->SetGizmoType(-1);
				break;
			case Key::W:
				m_Viewport->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
				break;
			case Key::E:
				m_Viewport->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
				break;
			case Key::R:
				m_Viewport->SetGizmoType(ImGuizmo::OPERATION::SCALE);
				break;

			// Scene shortcuts
			case Key::N:
			{
				if(control)
				{
					NewScene();
				}
			} break;
			case Key::O:
			{
				if(control)
				{
					OpenScene();
				}
			} break;
			case Key::S:
			{
				if(control && shift)
				{
					SaveAs();
				}
			} break;
		}

		return true;
	}

}