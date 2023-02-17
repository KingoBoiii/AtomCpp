#include "ATPCH.h"
#include "ScriptGlue.h"

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

	}

}


