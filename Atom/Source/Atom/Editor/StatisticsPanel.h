#pragma once

#include "EditorPanel.h"

namespace Atom
{
	
	class StatisticsPanel : public EditorPanel
	{
	public:
		virtual void OnImGuiRender(bool& isOpen) override;
	private:
		void DrawRendererStatistics();
	};

}
