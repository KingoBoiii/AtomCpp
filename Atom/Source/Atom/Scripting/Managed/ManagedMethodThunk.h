#pragma once

#include "Atom/Core/PlatformDetection.h"

#include "Atom/Scripting/ScriptUtils.h"
#include "Atom/Scripting/Managed/ManagedMethod.h"

#ifdef AT_PLATFORM_WINDOWS
#define AT_MONO_STDCALL __stdcall
#else
#define AT_MONO_STDCALL
#endif

extern "C" {
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoException MonoException;
}

namespace Atom
{

	template<typename... TParameterTypes>
	struct ManagedMethodThunk
	{
		typedef void(AT_MONO_STDCALL* MethodThunk)(MonoObject*, TParameterTypes... params, MonoException**);

		MethodThunk Method = nullptr;

		ManagedMethodThunk() = default;
		ManagedMethodThunk(ManagedMethod* managedMethod)
		{
			RegisterMethodThunk(managedMethod);
		}

		void Invoke(MonoObject* instance, TParameterTypes... params, MonoException** exception = nullptr) const
		{
			Method(instance, params..., exception);
		}

		void RegisterMethodThunk(ManagedMethod* managedMethod)
		{
			Method = (MethodThunk)ScriptUtils::GetUnmanagedMethodThunk(managedMethod);
		}
	};

}
