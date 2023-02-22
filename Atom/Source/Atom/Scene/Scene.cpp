#include "ATPCH.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "Atom/Scripting/ScriptEngine.h"
#include "Atom/Physics/2D/Physics2D.h"

#include "Atom/Renderer/Renderer2D.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Atom
{

	namespace Utils
	{

		static b2BodyType AtomBodyTypeToBox2D(Component::Rigidbody2D::BodyType bodyType)
		{
			switch(bodyType)
			{
				case Atom::Component::Rigidbody2D::BodyType::Static:	return b2_staticBody;
				case Atom::Component::Rigidbody2D::BodyType::Dynamic:	return b2_dynamicBody;
				case Atom::Component::Rigidbody2D::BodyType::Kinematic: return b2_kinematicBody;
			}

			AT_CORE_ASSERT(false, "Unknown body type!");
			return b2_staticBody;
		}

	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	template<typename TComponent>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<TComponent>();
		for(auto e : view)
		{
			UUID uuid = src.get<Component::Identifier>(e).ID;
			AT_CORE_ASSERT(enttMap.find(uuid) != enttMap.end());
			entt::entity dstEnttId = enttMap.at(uuid);

			auto& component = src.get<TComponent>(e);
			dst.emplace_or_replace<TComponent>(dstEnttId, component);
		}
	}

	template<typename TComponent>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if(src.HasComponent<TComponent>())
		{
			dst.AddOrReplaceComponent<TComponent>(src.GetComponent<TComponent>());
		}
	}

	Scene* Scene::Copy(Scene* other)
	{
		Scene* newScene = new Scene();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;

		std::unordered_map<UUID, entt::entity> enttMap;

		// Creat entities in new scene
		auto identifierView = srcSceneRegistry.view<Component::Identifier>();
		for(auto e : identifierView)
		{
			UUID uuid = srcSceneRegistry.get<Component::Identifier>(e).ID;
			const auto& name = srcSceneRegistry.get<Component::Identifier>(e).Name;
			Entity newEntity = newScene->CreateEntity(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except for Identifier)
		CopyComponent<Component::Transform>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Component::Camera>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Component::BasicRenderer>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Component::Script>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Component::Rigidbody2D>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Component::BoxCollider2D>(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
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

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		CopyComponentIfExists<Component::Transform>(newEntity, entity);
		CopyComponentIfExists<Component::Camera>(newEntity, entity);
		CopyComponentIfExists<Component::BasicRenderer>(newEntity, entity);
		CopyComponentIfExists<Component::Script>(newEntity, entity);
		CopyComponentIfExists<Component::Rigidbody2D>(newEntity, entity);
		CopyComponentIfExists<Component::BoxCollider2D>(newEntity, entity);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if(m_ViewportWidth == width && m_ViewportHeight == height)
		{
			return;
		}

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

	void Scene::OnEditorUpdate(float deltaTime, EditorCamera& editorCamera)
	{
		Renderer2D::BeginScene(editorCamera);

		{
			auto group = m_Registry.group<Component::Transform>(entt::get<Component::BasicRenderer>);
			for(auto entity : group)
			{
				auto [transform, basic] = group.get<Component::Transform, Component::BasicRenderer>(entity);

				Renderer2D::RenderQuad(transform.GetTransform(), basic.Color);
			}
		}

		Renderer2D::DrawString("Atom!", Font::GetDefaultFont(), glm::mat4(1.0f));
		Renderer2D::DrawString(R"(
struct VSInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
};

struct VSOutput
{
	float4 PositionCS : SV_POSITION;
	float4 Position : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	float4x4 ProjectionViewMatrix;
}

VSOutput VSMain(VSInput input)
{
	float4 pos = float4(input.Position, 1.0);

	VSOutput output = (VSOutput)0; // zero the memory first
	output.PositionCS = mul(ProjectionViewMatrix, pos);
	output.Position = pos;
	output.Color = input.Color;
	output.TexCoord = input.TexCoord;
	return output;
}

Texture2D FontAtlasTexture : register(t0);
SamplerState FontAtlasSampler : register(s0);

struct PSOutput
{
	float4 Color : SV_TARGET;
};

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float screenPxRange(float2 texCoord)
{
	float pxRange = 2.0f;

	float width, height, levels;
	FontAtlasTexture.GetDimensions(0, width, height, levels);

	float2 unitRange = float2(pxRange, pxRange) / float2(width, height);
	float2 screenTexSize = float2(1.0f, 1.0f) / fwidth(texCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

PSOutput PSMain(VSOutput input)
{
	float4 msdf = FontAtlasTexture.Sample(FontAtlasSampler, input.TexCoord);
	float3 msd = msdf.rgb;

	float sd = median(msd.r, msd.g, msd.b);

	float screenPxDistance = screenPxRange(input.TexCoord) * (sd - 0.5f);

	float opacity = clamp(screenPxDistance + 0.5f, 0.0f, 1.0f);
	if(opacity == 0.0f)
	{
		discard;
	}

	float4 bgColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		
	float4 color = lerp(bgColor, input.Color, opacity);
	if(color.a == 0.0f)
	{
		discard;
	}

	PSOutput output = (PSOutput)0;
	output.Color = color;
	return output;
})", Font::GetDefaultFont(), glm::mat4(1.0f));

		Renderer2D::EndScene();
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;

		// Physics 2D
		{
			Physics2D::OnRuntimeStart();

			auto view = m_Registry.view<Component::Rigidbody2D>();
			for(auto e : view)
			{
				Entity entity{ e, this };
				Physics2D::CreatePhysicsBody(entity);
			}
		}

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
		m_IsRunning = false;

		// Physics 2D
		{
			Physics2D::OnRuntimeStop();
		}

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
		if(!m_IsPaused || m_StepFrames-- > 0)
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

			// Physics 2D
			{
				Physics2D::Step(deltaTime);
				auto view = m_Registry.view<Component::Rigidbody2D>();
				for(auto e : view)
				{
					Entity entity{ e, this };

					Physics2D::OnRuntimeUpdate(entity);
				}
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

	void Scene::OnSimulationStart()
	{
		// Physics 2D
		{
			Physics2D::OnRuntimeStart();

			auto view = m_Registry.view<Component::Rigidbody2D>();
			for(auto e : view)
			{
				Entity entity{ e, this };
				Physics2D::CreatePhysicsBody(entity);
			}
		}
	}

	void Scene::OnSimulationStop()
	{
		// Physics 2D
		{
			Physics2D::OnRuntimeStop();
		}
	}

	void Scene::OnSimulationUpdate(float deltaTime, EditorCamera& editorCamera)
	{
		if(!m_IsPaused || m_StepFrames-- > 0)
		{
			// Physics 2D
			{
				Physics2D::Step(deltaTime);
				auto view = m_Registry.view<Component::Rigidbody2D>();
				for(auto e : view)
				{
					Entity entity{ e, this };

					Physics2D::OnRuntimeUpdate(entity);
				}
			}
		}

		// Renderer 2D
		Renderer2D::BeginScene(editorCamera);

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

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<Component::Identifier>();
		for(auto entity : view)
		{
			const auto& identifier = view.get<Component::Identifier>(entity);
			if(identifier.Name == name)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if(m_EntityMap.find(uuid) != m_EntityMap.end())
		{
			return { m_EntityMap[uuid], this };
		}
		return {};
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

	void Scene::Step(int frames)
	{
		m_StepFrames = frames;
	}

}