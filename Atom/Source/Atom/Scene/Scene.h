#pragma once
#include "Atom/Core/UUID.h"

#include "entt.hpp"

namespace Atom
{

	class ATOM_API Entity;

	class ATOM_API Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
	private:
		entt::registry m_Registry;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
	};

}
