#include "ATPCH.h"
#include "Entity.h"

namespace Atom
{

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}