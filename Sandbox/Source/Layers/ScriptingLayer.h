#pragma once
#include <Atom.h>

class ScriptingLayer : public Atom::Layer
{
public:
	ScriptingLayer() : Layer("Scripting Layer") { }
	virtual ~ScriptingLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
private:
	Atom::Renderer* m_Renderer = nullptr;
	Atom::Renderer2D* m_Renderer2D = nullptr;
	Atom::Scene* m_Scene = nullptr;

	Atom::ScriptEngine* m_ScriptEngine = nullptr;
};

