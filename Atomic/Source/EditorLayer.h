#pragma once
#include <Atom.h>

namespace Atomic
{
	
	class EditorLayer : public Atom::Layer
	{
	public:
		EditorLayer() : Layer("Atomic Editor Layer")
		{
		}

		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
	};

}
