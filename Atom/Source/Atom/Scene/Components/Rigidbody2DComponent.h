#pragma once
#include "Atom/Physics/2D/Physics2DBodyTypes.h"

namespace Atom::Component
{

	struct Rigidbody2D
	{
		PhysicsBodyType BodyType = PhysicsBodyType::Static;
		bool FixedRotation = false;

		bool AffectedByGravity = true;
		float GravityScale = 1.0f;

		Rigidbody2D() = default;
		Rigidbody2D(const Rigidbody2D& other) = default;
	};

}
