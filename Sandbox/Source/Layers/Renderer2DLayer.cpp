#include "Renderer2DLayer.h"

void Renderer2DLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();

	Atom::RendererOptions rendererOptions{ };
	rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
	rendererOptions.SwapChain = window->GetSwapChain();
	m_Renderer = Atom::RendererFactory::CreateRenderer(rendererOptions);
	m_Renderer->Initialize();

	m_Renderer2D = Atom::RendererFactory::CreateRenderer2D(m_Renderer);
	m_Renderer2D->Initialize();
}

void Renderer2DLayer::OnDetach()
{
	m_Renderer2D->Shutdown();
}

void Renderer2DLayer::OnUpdate(float deltaTime)
{
	m_Renderer->Clear();

	m_Renderer2D->BeginScene(m_Camera);

	m_Renderer2D->RenderQuad({ -1.5f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	m_Renderer2D->RenderQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
	m_Renderer2D->RenderQuad({ 1.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });

	m_Renderer2D->EndScene();
}
