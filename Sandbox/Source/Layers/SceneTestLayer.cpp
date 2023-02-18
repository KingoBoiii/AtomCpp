#include "SceneTestLayer.h"

void SceneTestLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();
	
	m_Scene = new Atom::Scene();
	auto cameraEntity = m_Scene->CreateEntity("Camera");
	auto& transform = cameraEntity.GetComponent<Atom::Component::Transform>();
	transform.Position = { 0.0f, 0.0f, 1.0f };
	cameraEntity.AddComponent<Atom::Component::Camera>();

	auto quadEntity = m_Scene->CreateEntity("Quad");
	quadEntity.AddComponent<Atom::Component::BasicRenderer>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

	m_Scene->OnViewportResize(window->GetWindowOptions().Width, window->GetWindowOptions().Height);
}

void SceneTestLayer::OnDetach()
{
	m_Scene->OnRuntimeStop();
	delete m_Scene;
}

void SceneTestLayer::OnUpdate(float deltaTime)
{
	Atom::Renderer::Clear();

	m_Scene->OnRuntimeUpdate(deltaTime);
}
