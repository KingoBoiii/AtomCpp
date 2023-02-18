#include "Renderer2DLayer.h"

void Renderer2DLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();
}

void Renderer2DLayer::OnDetach()
{
	m_Renderer2D->Shutdown();
}

void Renderer2DLayer::OnUpdate(float deltaTime)
{
	Atom::Renderer::Clear();

	Atom::Renderer2D::BeginScene(m_Camera);

	Atom::Renderer2D::RenderQuad({ -1.5f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	Atom::Renderer2D::RenderQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
	Atom::Renderer2D::RenderQuad({ 1.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });

	Atom::Renderer2D::EndScene();
}
