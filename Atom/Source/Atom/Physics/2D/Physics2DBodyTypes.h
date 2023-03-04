#pragma once
#include <string>

namespace Atom 
{

	enum class PhysicsBodyType 
	{
		Static = 0,
		Dynamic = 1,
		Kinematic = 2
	};

	namespace Utils 
	{

		static PhysicsBodyType AtomPhysicsBodyTypeFromString(const std::string& physicsBodyTypeString) 
		{
			if (physicsBodyTypeString == "Static")		return PhysicsBodyType::Static;
			if (physicsBodyTypeString == "Dynamic")		return PhysicsBodyType::Dynamic;
			if (physicsBodyTypeString == "Kinematic")	return PhysicsBodyType::Kinematic;

			AT_CORE_ASSERT(false);
			return PhysicsBodyType::Static;
		}

		static std::string AtomPhysicsBodyTypeToString(PhysicsBodyType physicsBodyType) 
		{
			switch (physicsBodyType)
			{
			case Atom::PhysicsBodyType::Static:		return "Static";
			case Atom::PhysicsBodyType::Dynamic:	return "Dynamic";
			case Atom::PhysicsBodyType::Kinematic:	return "Kinematic";
			default: break;
			}

			AT_CORE_ASSERT(false);
			return "Static";
		}

	}

}
