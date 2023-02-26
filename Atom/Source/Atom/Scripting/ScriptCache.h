#pragma once

#include "Managed/ManagedClass.h"

#define AT_CORE_CLASS(className)		::Atom::ScriptCache::GetManagedClassByName("Atom.".#className)
#define AT_CACHED_CLASS(className)		::Atom::ScriptCache::GetManagedClassByName(className)
#define AT_CACHED_CLASS_RAW(className)	::Atom::ScriptCache::GetManagedClassByName(className)->Class

namespace Atom
{

	class ScriptCache
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void ClearCache();
		static void CacheCoreClasses();

		static ManagedClass* GetManagedClassByName(const std::string& className);
	private:
		static void CacheClass(std::string_view className, MonoClass* monoClass);
	};

}
