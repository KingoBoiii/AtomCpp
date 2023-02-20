#include "EditorLayer.h"
#include <Atom/Scene/SceneSerializer.h>
#include <Atom/Utils/PlatformUtils.h>
#include <Atom/ImGui/ImGuiUtillities.h>

#include <imgui.h>

namespace Atomic
{

#define ATOM_SCENE_FILE_EXTENSIONS "atsc"
#define ATOM_SCENE_FILE_DIALOG_FILTER "Atom Scene (*.atsc)\0*.atsc\0"

	void EditorLayer::OnAttach()
	{
		Atom::Window* window = Atom::Application::Get().GetWindow();

		m_Scene = new Atom::Scene();

		Atom::FramebufferOptions framebufferOptions{ };
		framebufferOptions.ClearColor = new float[4] { 0.15f, 0.15f, 0.15f, 1.0f };
		framebufferOptions.Width = window->GetWidth();
		framebufferOptions.Height = window->GetHeight();
		m_Framebuffer = Atom::Framebuffer::Create(framebufferOptions);

		m_Viewport = new Viewport(m_Framebuffer);
		m_SceneHierarchyPanel = new Atom::SceneHierarchyPanel(m_Scene);

#if 0
		auto cameraEntity = m_Scene->CreateEntity("Camera");
		auto& transform = cameraEntity.GetComponent<Atom::Component::Transform>();
		transform.Position = { 0.0f, 0.0f, 0.1f };
		auto& camera = cameraEntity.AddComponent<Atom::Component::Camera>();

		auto quadEntity = m_Scene->CreateEntity("Quad");
		quadEntity.AddComponent<Atom::Component::BasicRenderer>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		quadEntity.AddComponent<Atom::Component::Script>("Sandbox.Player");
#endif

		m_Scene->OnRuntimeStart();
	}

	void EditorLayer::OnDetach()
	{
		m_Scene->OnRuntimeStop();
		delete m_Scene;
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		Atom::FramebufferOptions framebufferOptions = m_Framebuffer->GetOptions();
		if(m_Viewport->GetViewportSize().x > 0.0f && m_Viewport->GetViewportSize().y > 0.0f && (framebufferOptions.Width != m_Viewport->GetViewportSize().x || framebufferOptions.Height != m_Viewport->GetViewportSize().y))
		{
			const glm::vec2& viewportSize = m_Viewport->GetViewportSize();
			m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_Scene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		}

		m_Framebuffer->Bind();
		m_Framebuffer->Clear();

		m_Scene->OnRuntimeUpdate(deltaTime);

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

	void EditorLayer::OnEvent(Atom::Event& e)
	{
		Atom::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Atom::KeyPressedEvent>(AT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
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
					Atom::SceneSerializer serializer(m_Scene);
					serializer.Serialize(filepath);
				}
				if(ImGui::MenuItem("Load Example Scene"))
				{
					Atom::SceneSerializer serializer(m_Scene);
					serializer.Deserialize(filepath);

					m_Scene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Restart (W.I.P.)"))
				{
					Atom::Application::Get().Restart();
				}
				if(ImGui::MenuItem("Close"))
				{
					Atom::Application::Get().Close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::NewScene()
	{
		m_Scene = new Atom::Scene();
		m_Scene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
		m_SceneHierarchyPanel->SetSceneContext(m_Scene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = Atom::FileDialogs::OpenFile(ATOM_SCENE_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			AT_CORE_WARN("Cancelled open file dialog!");
			return;
		}

		m_Scene = new Atom::Scene();
		m_SceneHierarchyPanel->SetSceneContext(m_Scene);

		Atom::SceneSerializer serializer(m_Scene);
		auto d = serializer.Deserialize(filepath);

		m_Scene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
	}

	void EditorLayer::SaveAs()
	{
		std::string filepath = Atom::FileDialogs::SaveFile(ATOM_SCENE_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			AT_CORE_WARN("Cancelled save file dialog!");
			return;
		}

		Atom::SceneSerializer serializer(m_Scene);
		serializer.Serialize(filepath);
	}

	bool EditorLayer::OnKeyPressed(Atom::KeyPressedEvent& e)
	{
		// Shortcuts
		if(e.GetRepeatCount() > 0)
		{
			return false;
		}

		bool control = Atom::Input::IsKeyDown(Atom::Key::LeftControl) || Atom::Input::IsKeyDown(Atom::Key::RightControl);
		bool shift = Atom::Input::IsKeyDown(Atom::Key::LeftShift) || Atom::Input::IsKeyDown(Atom::Key::RightShift);

		switch(e.GetKeyCode())
		{
			case Atom::Key::N:
			{
				if(control)
				{
					NewScene();
				}
			} break;
			case Atom::Key::O:
			{
				if(control)
				{
					OpenScene();
				}
			} break;
			case Atom::Key::S:
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