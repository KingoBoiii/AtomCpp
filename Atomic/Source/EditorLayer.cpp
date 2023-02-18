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

		auto cameraEntity = m_Scene->CreateEntity("Camera");
		auto& transform = cameraEntity.GetComponent<Atom::Component::Transform>();
		transform.Position = { 0.0f, 0.0f, 1.0f };
		cameraEntity.AddComponent<Atom::Component::Camera>();

		auto quadEntity = m_Scene->CreateEntity("Quad");
		quadEntity.AddComponent<Atom::Component::BasicRenderer>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

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
	}

	void EditorLayer::OnGUI()
	{
#if 1
		static bool opened = true;
		ImGui::ShowDemoWindow(&opened);
#endif
	}

}