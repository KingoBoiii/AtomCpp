#pragma once

#include "Components/IdentifierComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "components/BasicRendererComponent.h"
#include "components/ScriptComponent.h"

namespace Atom
{

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<Component::Transform, Component::Camera, Component::BasicRenderer, Component::Script>;

}