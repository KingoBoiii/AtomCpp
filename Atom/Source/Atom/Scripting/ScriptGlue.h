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

#pragma region Scene

		void Scene_CreateEntity(MonoString* name, UUID* outEntityId);
		void Scene_DestroyEntity(UUID entityId);
		void Scene_FindEntityByName(MonoString* name, UUID* outEntityId);

#pragma endregion
		
#pragma region Entity

		bool Entity_HasComponent(UUID uuid, MonoReflectionType* monoReflectionType);

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

#pragma region Rigidbody 2D

		void Rigidbody2D_GetPosition(UUID uuid, glm::vec2* outPosition);
		void Rigidbody2D_SetPosition(UUID uuid, glm::vec2* position);

		void Rigidbody2D_SetLinearVelocity(UUID uuid, glm::vec2* velocity);

#pragma endregion

#pragma region Text

		void TextRenderer_GetTextString(UUID uuid, MonoString** outText);
		void TextRenderer_SetTextString(UUID uuid, MonoString* text);

		void TextRenderer_GetKerning(UUID uuid, float* kerning);
		void TextRenderer_SetKerning(UUID uuid, float kerning);
		
		void TextRenderer_GetLineSpacing(UUID uuid, float* lineSpacing);
		void TextRenderer_SetLineSpacing(UUID uuid, float lineSpacing);

		void TextRenderer_GetColor(UUID uuid, glm::vec4* outColor);
		void TextRenderer_SetColor(UUID uuid, glm::vec4* color);

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