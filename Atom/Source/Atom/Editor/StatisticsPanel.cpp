#include "ATPCH.h"
#include "StatisticsPanel.h"

#include "Atom/Renderer/Renderer2D.h"

#include <imgui.h>

namespace Atom
{

	void StatisticsPanel::OnImGuiRender(bool& isOpen)
	{
		ImGui::Begin("Statistics", &isOpen);

		ImGuiTabBarFlags tabFlags = ImGuiTabBarFlags_None;
		if(ImGui::BeginTabBar("StatisticsTab", tabFlags))
		{
			DrawRendererStatistics();

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void StatisticsPanel::DrawRendererStatistics()
	{
		if(ImGui::BeginTabItem("Renderer"))
		{
			const Renderer2D::Statistics& stats = Renderer2D::GetStats();
			
			ImGui::Text("Renderer2D Statistics:");
			ImGui::Text("Draw Calls: %i", stats.DrawCalls);
			ImGui::Text("Quad Count: %i", stats.QuadCount);
			
			ImGui::EndTabItem();
		}
	}

}