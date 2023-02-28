#pragma once

#include "Components/IdentifierComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "components/BasicRendererComponent.h"
#include "components/CircleRendererComponent.h"
#include "components/ScriptComponent.h"
#include "components/Rigidbody2DComponent.h"
#include "components/BoxCollider2DComponent.h"
#include "Components/TextComponent.h"

namespace Atom
{

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<Component::Identifier, Component::Transform, Component::Camera, 
										 Component::BasicRenderer, Component::CircleRenderer, Component::Script,
										 Component::Rigidbody2D, Component::BoxCollider2D, 
										 Component::Text>;

}