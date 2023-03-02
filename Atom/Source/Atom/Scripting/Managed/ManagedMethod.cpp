#include "ATPCH.h"
#include "ManagedMethod.h"

#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>

namespace Atom
{

	ManagedMethod::ManagedMethod(MonoMethod* rawMethod)
		: m_RawMethod(rawMethod)
	{
		Invalidate();
	}

	ManagedMethod::~ManagedMethod()
	{
		m_RawMethod = nullptr;
	}

	void ManagedMethod::Invalidate()
	{
		MonoMethodSignature* methodSignature = mono_method_signature(m_RawMethod);

		m_MethodName = mono_method_get_name(m_RawMethod);
		m_FullName = mono_method_full_name(m_RawMethod, false);
		m_ParameterCount = mono_signature_get_param_count(methodSignature);
	}

}