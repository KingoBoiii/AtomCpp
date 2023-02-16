#pragma once

#include "Components/IdentifierComponent.h"
#include "Components/TransformComponent.h"
#include "components/BasicRendererComponent.h"

namespace Atom
{

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<Component::Transform, Component::BasicRenderer>;

}