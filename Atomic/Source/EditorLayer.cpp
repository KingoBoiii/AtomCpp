#include "EditorLayer.h"

#include <imgui.h>

namespace Atomic
{

	void EditorLayer::OnAttach()
	{
		Atom::Window* window = Atom::Application::Get().GetWindow();

		Atom::RendererOptions rendererOptions{ };
		rendererOptions.ClearColor = new float[4] { 0.1f, 0.1f, 0.1f, 1.0f };
		rendererOptions.SwapChain = window->GetSwapChain();
		m_Renderer = Atom::RendererFactory::CreateRenderer(rendererOptions);
		m_Renderer->Initialize();
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
	}

	void EditorLayer::OnGUI()
	{
		m_Renderer->Clear();

		static bool opened = true;
		ImGui::ShowDemoWindow(&opened);
	}

}