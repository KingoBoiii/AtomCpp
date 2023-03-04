#pragma once

#include "EditorPanel.h"

namespace Atom
{
	
	class StatisticsPanel : public EditorPanel
	{
	public:
		virtual void DrawUI(bool& isOpen) override;
	private:
		void DrawRendererStatistics();
	};

}
