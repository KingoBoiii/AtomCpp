#pragma once
#include "Atom/Core/Base.h"
#include "Atom/Scene/Scene.h"
#include "Atom/Project/Project.h"

namespace Atom
{

	class EditorPanel
	{
	public:
		virtual ~EditorPanel() = default;

		virtual void OnImGuiRender(bool& isOpen) = 0;
		virtual void SetSceneContext(Scene* scene) { }
		virtual void OnProjectChanged(Project* project) { }
	};

}