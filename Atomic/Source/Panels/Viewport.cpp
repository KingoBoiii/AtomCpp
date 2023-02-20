#include "Viewport.h"

#include <imgui.h>

namespace Atomic
{

	Viewport::Viewport(Atom::Framebuffer* framebuffer)
		: m_Framebuffer(framebuffer)
	{
	}

	void Viewport::OnImGuiRender(bool& isOpen)
	{
		static bool opened = true;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", &opened, ImGuiWindowFlags_NoDecoration);

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		/*if(m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_Framebuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
		}*/

		ImGui::Image(m_Framebuffer->GetImage(), viewportPanelSize);

		ImGui::End();
		ImGui::PopStyleVar();
	}

}