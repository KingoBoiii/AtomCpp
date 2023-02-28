#pragma once
#include <string>

#include <mono/utils/mono-error.h>

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoAssembly MonoAssembly;
}

namespace Atom
{

	class ScriptUtils
	{
	public:
		static bool CheckMonoError(MonoError& error);

		static MonoString* EmptyMonoString();
		static MonoString* UTF8ToMonoString(const std::string& string);
		static std::string MonoStringToUTF8(MonoString* string);

		static void PrintAssemblyTypes(MonoAssembly* assembly);
	};

}
