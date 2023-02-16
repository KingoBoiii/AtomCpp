#pragma once
#include <Atom.h>

class Renderer2DLayer : public Atom::Layer
{
public:
	Renderer2DLayer() : Layer("Renderer2D") { }
	virtual ~Renderer2DLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
private:
	Atom::Renderer* m_Renderer = nullptr;
	Atom::Renderer2D* m_Renderer2D = nullptr;
	Atom::Camera m_Camera = Atom::Camera({ 0.0f, 0.0f, 1.0f });
};
