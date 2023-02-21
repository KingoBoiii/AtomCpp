#pragma once
#include "Atom/Core/UUID.h"
#include "Atom/Core/Input/Input.h"

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoReflectionType MonoReflectionType;
}

namespace Atom
{

	class ScriptGlue
	{
	public:
		static void RegisterComponents();
		static void RegisterInternalCalls();
	};

	namespace InternalCalls
	{
		
#pragma region Entity

		bool Entity_HasComponent(UUID uuid, MonoReflectionType* monoReflectionType);

		void Entity_FindEntityByName(MonoString* name, UUID* uuid);

		void Entity_GetScriptInstance(UUID uuid, MonoObject** monoObject);

#pragma endregion

#pragma region Identifier

		void Identifier_GetName(UUID uuid, MonoString** outName);
		void Identifier_SetName(UUID uuid, MonoString* name);

#pragma endregion


#pragma region Transform

		void Transform_GetPosition(UUID uuid, glm::vec3* outPosition);
		void Transform_SetPosition(UUID uuid, glm::vec3* position);

#pragma endregion

#pragma region Basic Renderer

		void BasicRenderer_GetColor(UUID uuid, glm::vec4* outColor);
		void BasicRenderer_SetColor(UUID uuid, glm::vec4* color);

#pragma endregion

#pragma region Input

		bool Input_IsKeyDown(KeyCode keycode);

#pragma endregion

#pragma region Log

		enum class LogLevel : int32_t
		{
			Trace = BIT(0),
			Debug = BIT(1),
			Info = BIT(2),
			Warn = BIT(3),
			Error = BIT(4),
			Critical = BIT(5)
		};

		void Log_LogMessage(LogLevel level, MonoString* message);

		void Log_Trace(MonoString* string);
		void Log_Information(MonoString* string);
		void Log_Warning(MonoString* string);
		void Log_Error(MonoString* string);

#pragma endregion


	}

}