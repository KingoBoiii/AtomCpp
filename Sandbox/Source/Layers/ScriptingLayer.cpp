#include "ScriptingLayer.h"

void ScriptingLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();
	
	m_Scene = new Atom::Scene();
	auto cameraEntity = m_Scene->CreateEntity("Camera");
	auto& transform = cameraEntity.GetComponent<Atom::Component::Transform>();
	transform.Position = { 0.0f, 0.0f, 1.0f };
	cameraEntity.AddComponent<Atom::Component::Camera>();

	auto quadEntity = m_Scene->CreateEntity("Quad");
	quadEntity.AddComponent<Atom::Component::BasicRenderer>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

	std::string playerClassName = "Sandbox.Player";
	const auto& entityClasses = Atom::ScriptEngine::GetEntityClasses();
	AT_ASSERT(entityClasses.find(playerClassName) != entityClasses.end());
	quadEntity.AddComponent<Atom::Component::Script>(playerClassName);
	
	m_Scene->OnViewportResize(window->GetWindowOptions().Width, window->GetWindowOptions().Height);
	m_Scene->OnRuntimeStart();
}

void ScriptingLayer::OnDetach()
{
	m_Scene->OnRuntimeStop();
	delete m_Scene;
}

void ScriptingLayer::OnUpdate(float deltaTime)
{
	Atom::Renderer::Clear();

	m_Scene->OnRuntimeUpdate(deltaTime);
}
