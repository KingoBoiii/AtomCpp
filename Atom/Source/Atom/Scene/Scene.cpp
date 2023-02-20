#include "ATPCH.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "Atom/Scripting/ScriptEngine.h"

#include "Atom/Renderer/Renderer2D.h"

namespace Atom
{

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		auto& identifier = entity.AddComponent<Component::Identifier>(uuid);
		identifier.Name = name.empty() ? "Entity" : name;

		entity.AddComponent<Component::Transform>();

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<Component::Camera>();
		for(auto entity : view)
		{
			auto& cameraComponent = view.get<Component::Camera>(entity);
			if(!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.SceneCamera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::OnRuntimeStart()
	{
		// Script
		{
			ScriptEngine::OnRuntimeStart(this);

			auto view = m_Registry.view<Component::Script>();
			for(auto e : view)
			{
				Entity entity{ e, this };
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		// Script
		{
			// TODO: Maybe call OnDestroyEntity?
			auto view = m_Registry.view<Component::Script>();
			for(auto e : view)
			{
				Entity entity{ e, this };
				ScriptEngine::OnDestroyEntity(entity);
			}

			ScriptEngine::OnRuntimeStop();
		}
	}

	void Scene::OnRuntimeUpdate(float deltaTime)
	{
		// Script 
		{
			auto view = m_Registry.view<Component::Script>();
			for(auto e : view)
			{
				Entity entity{ e, this };
				ScriptEngine::OnUpdateEntity(entity, deltaTime);
			}
		}

		// Renderer 2D
		{
			Camera* mainCamera = nullptr;
			glm::mat4 cameraTransform;
			{
				auto view = m_Registry.view<Component::Transform, Component::Camera>();
				for(auto entity : view)
				{
					auto [transform, camera] = view.get<Component::Transform, Component::Camera>(entity);

					if(camera.Primary)
					{
						mainCamera = &camera.SceneCamera;
						cameraTransform = transform.GetTransform();
						break;
					}
				}
			}

			if(mainCamera)
			{
				Renderer2D::BeginScene(*mainCamera, cameraTransform);

				{
					auto group = m_Registry.group<Component::Transform>(entt::get<Component::BasicRenderer>);
					for(auto entity : group)
					{
						auto [transform, basic] = group.get<Component::Transform, Component::BasicRenderer>(entity);

						Renderer2D::RenderQuad(transform.GetTransform(), basic.Color);
					}
				}

				Renderer2D::EndScene();
			}
		}
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if(m_EntityMap.find(uuid) != m_EntityMap.end())
		{
			return { m_EntityMap[uuid], this };
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<Component::Camera>();
		for(auto entity : view)
		{
			const auto& camera = view.get<Component::Camera>(entity);
			if(camera.Primary)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

}