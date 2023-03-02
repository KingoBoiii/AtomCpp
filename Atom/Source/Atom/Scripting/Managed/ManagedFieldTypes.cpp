#include "ATPCH.h"
#include "ManagedFieldTypes.h"

#include <mono/metadata/class.h>

namespace Atom::Utils
{

	ManagedFieldType MonoTypeToScriptFieldType(MonoType* monoType)
	{
		std::string typeName = mono_type_get_name(monoType);

		auto it = s_ScriptFieldTypeMap.find(typeName);
		if(it == s_ScriptFieldTypeMap.end())
		{
			return ManagedFieldType::None;
		}

		return it->second;
	}

}