#pragma once
#include <Atom.h>

class SceneTestLayer : public Atom::Layer
{
public:
	SceneTestLayer() : Layer("Scene Test") { }
	virtual ~SceneTestLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
private:
	Atom::Scene* m_Scene = nullptr;
};
