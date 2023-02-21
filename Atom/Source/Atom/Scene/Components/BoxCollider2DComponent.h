#pragma once

#include <glm/glm.hpp>

namespace Atom::Component
{

	struct BoxCollider2D
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 1.0f, 1.0f };
		bool IsTrigger = false;

		float Density = 1.0f;
		float Friction = 0.2f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		BoxCollider2D() = default;
		BoxCollider2D(const BoxCollider2D& other) = default;
	};

}
