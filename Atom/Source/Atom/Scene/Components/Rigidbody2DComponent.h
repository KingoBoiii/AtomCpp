#pragma once

namespace Atom::Component
{

	struct Rigidbody2D
	{
		enum class BodyType
		{
			Static = 0,
			Dynamic = 1,
			Kinematic = 2
		};

		BodyType Type = BodyType::Static;
		bool FixedRotation = false;
		
		void* RuntimeBody = nullptr;

		Rigidbody2D() = default;
		Rigidbody2D(const Rigidbody2D& other) = default;
	};

}
