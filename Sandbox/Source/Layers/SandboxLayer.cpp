#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
	Atom::Window* window = Atom::Application::Get().GetWindow();

	Atom::RendererOptions rendererOptions{ };
	rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
	rendererOptions.SwapChain = window->GetSwapChain();
	m_Renderer = Atom::RendererFactory::Create(rendererOptions);
	m_Renderer->Initialize();
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate()
{
	m_Renderer->Clear();
}
