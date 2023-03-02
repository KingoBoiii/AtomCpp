#pragma once
#include "Atom/Scripting/Managed/ManagedClass.h"
#include "Atom/Scripting/Managed/ManagedMethod.h"

#include <string>
#include <mono/utils/mono-error.h>

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoObject MonoObject;
}

namespace Atom
{

	class ScriptUtils
	{
	public:
		// NOTE: Call parameters less constructor!
		static MonoObject* InstantiateManagedClass(ManagedClass* managedClass);
		static void InvokeManagedMethod(ManagedMethod* managedMethod, void* instance, void** parameters = nullptr);

		static bool CheckMonoError(MonoError& error);
		static void HandleException(MonoObject* exception);

		static MonoString* EmptyMonoString();
		static MonoString* UTF8ToMonoString(const std::string& string);
		static std::string MonoStringToUTF8(MonoString* string);

		static void PrintAssemblyTypes(MonoAssembly* assembly);

		static void* GetUnmanagedMethodThunk(ManagedMethod* managedMethod);
	};

}
