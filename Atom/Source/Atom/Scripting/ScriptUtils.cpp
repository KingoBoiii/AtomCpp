#include "ATPCH.h"
#include "ScriptUtils.h"
#include "ScriptEngine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>

namespace Atom
{

	bool ScriptUtils::CheckMonoError(MonoError& error)
	{
		bool hasError = !mono_error_ok(&error);

		if(hasError)
		{
			uint16_t errorCode = mono_error_get_error_code(&error);
			const char* errorMessage = mono_error_get_message(&error);

			AT_CORE_ERROR("[ScriptEngine] Mono Error!");
			AT_CORE_ERROR("[ScriptEngine] \tError Code: {}", errorCode);
			AT_CORE_ERROR("[ScriptEngine] \tError Message: {}", errorMessage);

			mono_error_cleanup(&error);
			AT_CORE_ASSERT(false);
		}

		return hasError;
	}

	MonoString* ScriptUtils::EmptyMonoString()
	{
		return mono_string_empty(ScriptEngine::GetAppDomain());
	}

	MonoString* ScriptUtils::UTF8ToMonoString(const std::string& string)
	{
		return mono_string_new(ScriptEngine::GetAppDomain(), string.c_str());
	}

	std::string ScriptUtils::MonoStringToUTF8(MonoString* string)
	{
		if(string == nullptr || mono_string_length(string) == 0)
		{
			return "";
		}

		MonoError error;
		char* utf8CStr = mono_string_to_utf8_checked(string, &error);
		if(CheckMonoError(error))
		{
			return "";
		}

		std::string result(utf8CStr);
		
		mono_free(utf8CStr);

		return result;
	}

	void ScriptUtils::PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for(int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			AT_CORE_TRACE("{}.{}", nameSpace, name);
		}
	}

}