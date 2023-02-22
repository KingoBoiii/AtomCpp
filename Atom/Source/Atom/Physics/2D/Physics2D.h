#pragma once

class b2Body;

namespace Atom
{

	struct Physics2DConfig
	{
		int32_t VelocityIterations = 6;
		int32_t PositionIterations = 2;
	};

	class Entity;

	class Physics2D
	{
	public:
		static void Initialize(const Physics2DConfig& config = Physics2DConfig());
		static void Shutdown();

		static void Step(float deltaTime);

		static void OnRuntimeStart();
		static void OnRuntimeStop();
		static void OnRuntimeUpdate(Entity entity);

		static void CreatePhysicsBody(Entity entity);
		
		static void SetTransform(const glm::vec2& position, Entity entity);
		static glm::vec2 GetTransform(Entity entity);

		static b2Body* GetBox2DBody(Entity entity);
	};

}
