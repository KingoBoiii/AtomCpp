#include "ATPCH.h"
#include "ContactListener.h"

#include "Atom/Scene/Entity.h"
#include "Atom/Scripting/ScriptEngine.h"

#include <box2d/b2_contact.h>

namespace Atom
{

	void ContactListener::BeginContact(b2Contact* contact)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		AT_CORE_ASSERT(scene);
		
		UUID entityIdA = (UUID)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		UUID entityIdB = (UUID)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		Entity entityA = scene->GetEntityByUUID(entityIdA);
		Entity entityB = scene->GetEntityByUUID(entityIdB);

#if 0
		ScriptEngine::InvokeOnCollection2DEnter(entityA, entityB);
		ScriptEngine::InvokeOnCollection2DEnter(entityB, entityA);
#endif

		//AT_CORE_TRACE("Is Touching: {}", contact->IsTouching());
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		AT_CORE_ASSERT(scene);

		UUID entityIdA = (UUID)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		UUID entityIdB = (UUID)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		Entity entityA = scene->GetEntityByUUID(entityIdA);
		Entity entityB = scene->GetEntityByUUID(entityIdB);

#if 0
		ScriptEngine::InvokeOnCollection2DExit(entityA, entityB);
		ScriptEngine::InvokeOnCollection2DExit(entityB, entityA);
#endif

		//AT_CORE_TRACE("Is Touching: {}", contact->IsTouching());
	}

}