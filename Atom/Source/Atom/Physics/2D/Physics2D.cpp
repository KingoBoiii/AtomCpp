#include "ATPCH.h"
#include "Physics2D.h"
#include "ContactListener.h"

#include "Atom/Scene/Entity.h"

#include "box2d/b2_world.h"
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Atom
{

	namespace Utils
	{

		template<typename ColliderComponent>
		static void CreatePhysicsFixtures(Entity entity, Component::Transform& transform)
		{
			if(entity.HasComponent<ColliderComponent>())
			{
				auto& collider = entity.GetComponent<ColliderComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox((collider.Size.x * transform.Scale.x) * 0.5f, (collider.Size.y * transform.Scale.y) * 0.5f, { collider.Offset.x, collider.Offset.y }, 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = collider.Density;
				fixtureDef.friction = collider.Friction;
				fixtureDef.restitution = collider.Restitution;
				fixtureDef.restitutionThreshold = collider.RestitutionThreshold;
				fixtureDef.isSensor = collider.IsTrigger;

				b2Fixture* fixture = s_PhysicsData->EntityPhysicsBodies[entity.GetUUID()]->CreateFixture(&fixtureDef);
				//b2Fixture* fixture = Physics2D::GetBox2DBody(entity)->CreateFixture(&fixtureDef);
#if 0
				b2Fixture* fixture = body->CreateFixture(&fixtureDef);
#endif
				s_PhysicsData->EntityPhysicsFixtures[entity.GetUUID()] = fixture;
				collider.RuntimeFixture = fixture;
			}
		}

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

	struct Phyiscs2DData
	{
		bool Initialized = false;
		Physics2DConfig Config;

		b2World* World = nullptr;

		std::unordered_map<UUID, b2Body*> EntityPhysicsBodies;
		std::unordered_map<UUID, b2Fixture*> EntityPhysicsFixtures;
	};

	static Phyiscs2DData* s_PhysicsData = nullptr;

	void Physics2D::Initialize(const Physics2DConfig& config)
	{
		s_PhysicsData = new Phyiscs2DData();
		s_PhysicsData->Config = config;

		s_PhysicsData->Initialized = true;
	}

	void Physics2D::Shutdown()
	{
		s_PhysicsData->Initialized = false;

		delete s_PhysicsData;
		s_PhysicsData = nullptr;
	}

	void Physics2D::Step(float deltaTime)
	{
		AT_CORE_ASSERT(s_PhysicsData->Initialized, "Physics2D is not initialized!");

		s_PhysicsData->World->Step(deltaTime, s_PhysicsData->Config.VelocityIterations, s_PhysicsData->Config.PositionIterations);
	}

	void Physics2D::OnRuntimeStart()
	{
		AT_CORE_ASSERT(s_PhysicsData->Initialized, "Physics2D is not initialized!");

		s_PhysicsData->World = new b2World({ 0.0f, -9.81f });
		s_PhysicsData->World->SetContactListener(new ContactListener());
	}

	void Physics2D::OnRuntimeStop()
	{
		AT_CORE_ASSERT(s_PhysicsData->Initialized, "Physics2D is not initialized!");

		delete s_PhysicsData->World;
		s_PhysicsData->World = nullptr;
	}

	void Physics2D::OnRuntimeUpdate(Entity entity)
	{
		auto& transform = entity.GetComponent<Component::Transform>();
		auto& rb2d = entity.GetComponent<Component::Rigidbody2D>();

		b2Body* body = s_PhysicsData->EntityPhysicsBodies[entity.GetUUID()]; // (b2Body*)rb2d.RuntimeBody;
		const auto& position = body->GetPosition();

		transform.Position.x = position.x;
		transform.Position.y = position.y;
		transform.Rotation.z = body->GetAngle();
	}

	void Physics2D::CreatePhysicsBody(Entity entity)
	{
		auto& transform = entity.GetComponent<Component::Transform>();
		auto& rb2d = entity.GetComponent<Component::Rigidbody2D>();

		b2BodyDef bodyDef{};
		bodyDef.type = Utils::AtomBodyTypeToBox2D(rb2d.Type);
		bodyDef.fixedRotation = rb2d.FixedRotation;
		bodyDef.position.Set(transform.Position.x, transform.Position.y);
		bodyDef.angle = transform.Rotation.z;
		bodyDef.userData.pointer = (uintptr_t)entity.GetUUID();

		b2Body* body = s_PhysicsData->World->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);

		s_PhysicsData->EntityPhysicsBodies[entity.GetUUID()] = body;
		rb2d.RuntimeBody = body;

		Utils::CreatePhysicsFixtures<Component::BoxCollider2D>(entity, transform);
	}

	b2Body* Physics2D::GetBox2DBody(Entity entity)
	{
		AT_CORE_ASSERT(s_PhysicsData->EntityPhysicsBodies.find(entity.GetUUID()) != s_PhysicsData->EntityPhysicsBodies.end());
		return s_PhysicsData->EntityPhysicsBodies[entity.GetUUID()];
	}

}