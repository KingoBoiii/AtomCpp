#pragma once

#include <Atom/Editor/EditorPanel.h>

namespace Atom
{
	
	class ProjectExplorer : public EditorPanel
	{
	public:
		virtual void OnImGuiRender(bool& isOpen) override;
		virtual void OnProjectChanged(Project* project) override;
	};

}
