#include "ATPCH.h"
#include "ScriptGlue.h"

#include "Atom/Scene/Scene.h"
#include "Atom/Scene/Entity.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace Atom
{

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define AT_ADD_INTERNAL_CALL(icall) mono_add_internal_call("Atom.InternalCalls::"#icall, (void*)InternalCalls::icall)

	template<typename Component>
	static void RegisterComponent()
	{
		std::string_view typeName = typeid(Component).name();

		size_t pos = typeName.find_last_of(":");
		std::string_view structName = typeName.substr(pos + 1);
		std::string managedTypeName = fmt::format("Atom.{}", structName);

		MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
		if(!managedType)
		{
			AT_CORE_ERROR("Could not find managed component type: {}", managedTypeName);
			return;
		}
		s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		([]()
		{
			RegisterComponent<Component>();
		}(), ...);
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterInternalCalls()
	{
		AT_ADD_INTERNAL_CALL(Entity_HasComponent);

		AT_ADD_INTERNAL_CALL(Transform_GetPosition);
		AT_ADD_INTERNAL_CALL(Transform_SetPosition);

		AT_ADD_INTERNAL_CALL(Input_IsKeyDown);

		AT_ADD_INTERNAL_CALL(Log_LogMessage);
		AT_ADD_INTERNAL_CALL(Log_Trace);
		AT_ADD_INTERNAL_CALL(Log_Information);
		AT_ADD_INTERNAL_CALL(Log_Warning);
		AT_ADD_INTERNAL_CALL(Log_Error);
	}

	namespace InternalCalls
	{

#pragma region Entity

		bool Entity_HasComponent(UUID uuid, MonoReflectionType* monoReflectionType)
		{
			Scene* scene = ScriptEngine::GetScene();
			AT_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(uuid);
			AT_CORE_ASSERT(entity);

			MonoType* managedType = mono_reflection_type_get_type(monoReflectionType);
			AT_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "Entity_HasComponent: No function registered for this type!");

			return s_EntityHasComponentFuncs.at(managedType)(entity);
		}

#pragma endregion

#pragma region Transform

		void Transform_GetPosition(UUID uuid, glm::vec3* outPosition)
		{
			Scene* scene = ScriptEngine::GetScene();
			AT_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(uuid);
			AT_CORE_ASSERT(entity);

			*outPosition = entity.GetComponent<Component::Transform>().Position;
		}

		void Transform_SetPosition(UUID uuid, glm::vec3* position)
		{
			Scene* scene = ScriptEngine::GetScene();
			AT_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(uuid);
			AT_CORE_ASSERT(entity);

			entity.GetComponent<Component::Transform>().Position = *position;
		}

#pragma endregion

#pragma region Input

		bool Input_IsKeyDown(KeyCode keycode)
		{
			return Input::IsKeyDown(keycode);
		}

#pragma endregion


#pragma region Log

		void Log_LogMessage(LogLevel level, MonoString* message)
		{
			char* cStr = mono_string_to_utf8(message);
			std::string messageStr(cStr);
			mono_free(cStr);

			switch(level)
			{
				case LogLevel::Trace:
					AT_TRACE(messageStr);
					break;
				case LogLevel::Debug:
					AT_INFO(messageStr);
					break;
				case LogLevel::Info:
					AT_INFO(messageStr);
					break;
				case LogLevel::Warn:
					AT_WARN(messageStr);
					break;
				case LogLevel::Error:
					AT_ERROR(messageStr);
					break;
				case LogLevel::Critical:
					AT_CRITICAL(messageStr);
					break;
			}
		}

		void Log_Trace(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);

			AT_TRACE("{}", str);
		}

		void Log_Information(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);

			AT_INFO("{}", str);
		}

		void Log_Warning(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);

			AT_WARN("{}", str);
		}

		void Log_Error(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);

			AT_ERROR("{}", str);
		}

#pragma endregion

	}

}


