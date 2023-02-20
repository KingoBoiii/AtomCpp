#include "EditorLayer.h"

#include <Atom/Renderer/IRenderer.h>
#include <Atom/Renderer/RendererContext.h>

#include <Atom/Renderer/SwapChain.h>

#include <imgui.h>

namespace Atomic
{

	void EditorLayer::OnAttach()
	{
		Atom::Window* window = Atom::Application::Get().GetWindow();

		m_Scene = new Atom::Scene();

		Atom::FramebufferOptions framebufferOptions{ };
		framebufferOptions.ClearColor = new float[4] { 0.15f, 0.15f, 0.15f, 1.0f };
		framebufferOptions.Width = window->GetWidth();
		framebufferOptions.Height = window->GetHeight();
		m_Framebuffer = Atom::Framebuffer::Create(framebufferOptions);

		auto cameraEntity = m_Scene->CreateEntity("Camera");
		auto& transform = cameraEntity.GetComponent<Atom::Component::Transform>();
		transform.Position = { 0.0f, 0.0f, 0.1f };
		auto& camera = cameraEntity.AddComponent<Atom::Component::Camera>();

		auto quadEntity = m_Scene->CreateEntity("Quad");
		quadEntity.AddComponent<Atom::Component::BasicRenderer>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		quadEntity.AddComponent<Atom::Component::Script>("Sandbox.Player");

		m_Viewport = new Viewport(m_Framebuffer);
		m_SceneHierarchyPanel = new Atom::SceneHierarchyPanel(m_Scene);

		//m_Scene->OnViewportResize(window->GetWidth(), window->GetHeight());
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
		DrawDockSpace();
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

	void EditorLayer::DrawDockSpace()
	{
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if(opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		{
			window_flags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if(opt_fullscreen)
		{
			ImGui::PopStyleVar(2);
		}

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
	}

	void EditorLayer::DrawTopMenuBar()
	{
		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
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

}