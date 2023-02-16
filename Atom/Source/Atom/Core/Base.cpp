#include "ATPCH.h"
#include "Base.h"

#include "Log.h"

namespace Atom
{

	void InitializeEngine()
	{
		Log::Initialize();

		AT_CORE_TRACE("Atom initializing");
	}

	void ShutdownEngine()
	{
		AT_CORE_TRACE("Atom shutting down");

		Log::Shutdown();
	}

}