#pragma once
#include <Atom.h>

class SandboxLayer : public Atom::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox Layer")
	{
	}
	virtual ~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
private:
	Atom::Renderer* m_Renderer = nullptr;
};
