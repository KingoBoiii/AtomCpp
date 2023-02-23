#pragma once
#include "EditorPanel.h"

namespace Atom
{

	class ScriptEngineInspectorPanel : public EditorPanel
	{
	public:
		virtual void OnImGuiRender(bool& isOpen) override;
		virtual void OnProjectChanged(Project* project) override;
	};

}
