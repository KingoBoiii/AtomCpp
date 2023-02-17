#pragma once

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoReflectionType MonoReflectionType;
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
		
#pragma region Log

		void Log_Trace(MonoString* string);
		void Log_Information(MonoString* string);
		void Log_Warning(MonoString* string);
		void Log_Error(MonoString* string);

#pragma endregion


	}

}