#pragma once
#include "Base.h"

namespace Atom
{

	struct ATOM_API ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			// TODO: ASSERT HERE!
			return Args[index];
		}
	};

}