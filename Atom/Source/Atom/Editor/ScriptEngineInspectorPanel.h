#pragma once
#include "EditorPanel.h"

namespace Atom
{

	class ScriptEngineInspectorPanel : public EditorPanel
	{
	public:
		virtual void DrawUI(bool& isOpen) override;
		virtual void OnProjectChanged(Project* project) override;
	};

}
