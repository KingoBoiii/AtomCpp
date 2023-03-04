#pragma once
#include "EditorPanel.h"

namespace Atom
{

	class ProjectSettingsPanel : public EditorPanel
	{
	public:
		virtual void DrawUI(bool& isOpen) override;
		virtual void OnProjectChanged(Project* project) override;
	private:
		void ShowProjectSettings();
		void ShowScriptingSettings();
	};

}
