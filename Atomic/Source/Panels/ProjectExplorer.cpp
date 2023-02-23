#include "ProjectExplorer.h"

#include <imgui.h>

namespace Atom
{

	void ProjectExplorer::OnImGuiRender(bool& isOpen)
	{
		ImGui::Begin("Project Explorer", &isOpen);

		

		ImGui::End();
	}

	void ProjectExplorer::OnProjectChanged(Project* project)
	{
	}

}