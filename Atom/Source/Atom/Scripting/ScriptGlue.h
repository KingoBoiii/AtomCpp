#pragma once
#include "Atom/Core/UUID.h"

extern "C" {
	typedef struct _MonoString MonoString;
}

namespace Atom
{

	class ScriptGlue
	{
	public:
		static void RegisterInternalCalls();
	};

	namespace InternalCalls
	{
		
#pragma region Entity

		void Entity_GetPosition(UUID uuid, glm::vec3* outPosition);
		void Entity_SetPosition(UUID uuid, glm::vec3* position);

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