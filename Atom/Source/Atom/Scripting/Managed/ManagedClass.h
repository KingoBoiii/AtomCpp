#pragma once
#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
}

namespace Atom
{

	struct ManagedClass
	{
		uint32_t Id;
		std::string FullName;
		int32_t Size;

		MonoClass* Class = nullptr;

		ManagedClass() = default;
		~ManagedClass()
		{
			Class = nullptr;
		}
	};

}
