#include "EditorLayer.h"

#include <Atom/Renderer/IRenderer.h>
#include <Atom/Renderer/RendererContext.h>

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

		m_Texture = Atom::Texture2D::Create("Assets/Textures/dickbutt.jpg");

		auto cameraEntity = m_Scene->CreateEntity("Camera");
		auto& transform = cameraEntity.GetComponent<Atom::Component::Transform>();
		transform.Position = { 0.0f, 0.0f, 1.0f };
		cameraEntity.AddComponent<Atom::Component::Camera>();

		auto quadEntity = m_Scene->CreateEntity("Quad");
		quadEntity.AddComponent<Atom::Component::BasicRenderer>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		quadEntity.AddComponent<Atom::Component::Script>("Sandbox.Player");

		m_Scene->OnViewportResize(window->GetWidth(), window->GetHeight());
		m_Scene->OnRuntimeStart();
	}

	void EditorLayer::OnDetach()
	{
		m_Scene->OnRuntimeStop();
		delete m_Scene;
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		Atom::Renderer::Clear();

		m_Scene->OnRuntimeUpdate(deltaTime);

		m_Framebuffer->Bind();
		m_Framebuffer->Clear();

		m_Scene->OnRuntimeUpdate(deltaTime);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnGUI()
	{
		ImGui::Begin("Test");

		ImGui::Image(m_Framebuffer->GetImage(), { 1280.0f, 720.0f });

		ImGui::End();

		ImGui::Image(m_Texture->GetTexture(), { (float)m_Texture->GetWidth(), (float)m_Texture->GetHeight() }, { 0, 1 }, { 1, 0 });
		
#if 1
		static bool opened = true;
		ImGui::ShowDemoWindow(&opened);
#endif
	}

}