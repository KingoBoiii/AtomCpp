#include "ATPCH.h"
#include "ScriptClass.h"
#include "ScriptEngine.h"

#include <mono/metadata/object.h>

namespace Atom
{

	ScriptClass::ScriptClass(MonoImage* monoImage, const std::string& classNameSpace, const std::string& className)
		: m_MonoImage(monoImage), m_ClassNamespace(classNameSpace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(monoImage, classNameSpace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate() const
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, int parameterCount) const
	{
		return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* monoMethod, void** parameters)
	{
		return mono_runtime_invoke(monoMethod, instance, parameters, nullptr);
	}

}