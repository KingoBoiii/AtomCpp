#pragma once
#include "Atom/Core/Layer.h"
#include "Atom/Events/Event.h"

namespace Atom
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

	protected:
		void SetDarkThemeColors();
	};

}