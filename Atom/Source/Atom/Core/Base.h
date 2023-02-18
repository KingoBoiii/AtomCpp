#pragma once

#include "PlatformDetection.h"

#ifdef ATOM_STATIC_LIB
	#define ATOM_API
#else
	#ifdef ATOM_EXPORT_DLL
		#define ATOM_API __declspec(dllexport)
	#else
		#define ATOM_API __declspec(dllimport)
	#endif
#endif

namespace Atom
{

	void ATOM_API InitializeEngine();
	void ATOM_API ShutdownEngine();

}

#define AT_EXPAND_MACRO(x) x
#define AT_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define AT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define interface struct

#include "Assertion.h"