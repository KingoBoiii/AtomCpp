#include "SceneTestLayer.h"

void SceneTestLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();

	Atom::RendererOptions rendererOptions{ };
	rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
	rendererOptions.SwapChain = window->GetSwapChain();
	m_Renderer = Atom::RendererFactory::CreateRenderer(rendererOptions);
	m_Renderer->Initialize();

	m_Renderer2D = Atom::RendererFactory::CreateRenderer2D(m_Renderer);
	m_Renderer2D->Initialize();

	m_Scene = new Atom::Scene(m_Renderer2D);
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
	delete m_Scene;
	delete m_Renderer2D;
	delete m_Renderer;
}

void SceneTestLayer::OnUpdate(float deltaTime)
{
	m_Renderer->Clear();

	m_Scene->OnUpdateRuntime();
}
