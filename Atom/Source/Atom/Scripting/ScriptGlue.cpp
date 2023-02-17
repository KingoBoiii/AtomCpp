#include "ATPCH.h"
#include "ScriptGlue.h"

#include "Atom/Scene/Scene.h"
#include "Atom/Scene/Entity.h"

#include <mono/metadata/object.h>

namespace Atom
{

#define AT_ADD_INTERNAL_CALL(icall) mono_add_internal_call("Atom.InternalCalls::"#icall, (void*)InternalCalls::icall)

	static void CppFunction()
	{
		AT_CORE_TRACE("Hello from C++!");
	}

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);

		AT_CORE_TRACE("Native Log: {} ({})", str, parameter);
	}

	static void NativeLog_Vector(glm::vec3* vector)
	{
		AT_CORE_WARN("Vector3: {}", *vector);
	}

	void ScriptGlue::RegisterInternalCalls()
	{
		AT_ADD_INTERNAL_CALL(Entity_GetPosition);
		AT_ADD_INTERNAL_CALL(Entity_SetPosition);

		AT_ADD_INTERNAL_CALL(Input_IsKeyDown);

		AT_ADD_INTERNAL_CALL(Log_LogMessage);
		AT_ADD_INTERNAL_CALL(Log_Trace);
		AT_ADD_INTERNAL_CALL(Log_Information);
		AT_ADD_INTERNAL_CALL(Log_Warning);
		AT_ADD_INTERNAL_CALL(Log_Error);

		mono_add_internal_call("Atom.Main::CppFunction", CppFunction);
		mono_add_internal_call("Atom.Main::NativeLog", NativeLog);
		mono_add_internal_call("Atom.Main::NativeLog_Vector3", NativeLog_Vector);
	}

	namespace InternalCalls
	{
		
#pragma region Entity

		void Entity_GetPosition(UUID uuid, glm::vec3* outPosition)
		{
			Scene* scene = ScriptEngine::GetScene();

			Entity entity = scene->GetEntityByUUID(uuid);

			*outPosition = entity.GetComponent<Component::Transform>().Position;
		}

		void Entity_SetPosition(UUID uuid, glm::vec3* position)
		{
			Scene* scene = ScriptEngine::GetScene();

			Entity entity = scene->GetEntityByUUID(uuid);

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


