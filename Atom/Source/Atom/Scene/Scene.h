#pragma once
#include "Atom/Core/UUID.h"
#include "Atom/Editor/EditorCamera.h"

#include "entt.hpp"


namespace Atom
{

	class ATOM_API Entity;

	class ATOM_API Scene
	{
	public:
		Scene();
		~Scene();

		void OnRigidbody2DComponent(entt::registry& registry, entt::entity e);

		static Scene* Copy(Scene* other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void DuplicateEntity(Entity entity);

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnEditorUpdate(float deltaTime, EditorCamera& editorCamera);

		void OnRuntimeStart();
		void OnRuntimeStop();
		void OnRuntimeUpdate(float deltaTime);

		void OnSimulationStart();
		void OnSimulationStop();
		void OnSimulationUpdate(float deltaTime, EditorCamera& editorCamera);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);
		Entity GetPrimaryCameraEntity();

		bool IsEntityValid(Entity entity) const;
		bool IsEntityValid(UUID entityId) const;
		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }

		void Step(int frames = 1);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
