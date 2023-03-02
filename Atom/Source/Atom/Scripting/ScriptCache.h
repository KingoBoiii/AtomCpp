#pragma once

#include "Atom/Scripting/Assembly/AssemblyInfo.h"
#include "Atom/Scripting/Managed/ManagedClass.h"
#include "Atom/Scripting/Managed/ManagedMethod.h"

#define AT_CORE_CLASS(className)					::Atom::ScriptCache::GetManagedClassByName(fmt::format("{}.{}", "Atom", ##className))
#define AT_CACHED_CLASS(className)					::Atom::ScriptCache::GetManagedClassByName(className)
#define AT_CACHED_CLASS_RAW(className)				::Atom::ScriptCache::GetManagedClassByName(className)->GetRawClass()

#define AT_CACHED_ENTITY_CLASS(className)			::Atom::ScriptCache::GetManagedEntityClassByName(className)
#define AT_CACHED_METHOD(managedClass, methodName)	::Atom::ScriptCache::GetManagedMethodByName(managedClass, methodName)

extern "C" {
	typedef struct _MonoObject MonoObject;
}

namespace Atom
{

	class ScriptCache
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void ClearCache();
		static void CacheCoreClasses();

		static void CacheAssemblyClasses(AssemblyInfo* assemblyInfo);

		static ManagedMethod* GetManagedMethodByName(ManagedClass* managedClass, const std::string& methodName);
		static ManagedClass* GetManagedEntityClassByName(const std::string& className);
		static ManagedClass* GetManagedClassByName(const std::string& className);

		static void InvokeEntityStart(MonoObject* instance);
		static void InvokeEntityDestroy(MonoObject* instance);
		static void InvokeEntityUpdate(MonoObject* instance, float deltaTime);
	private:
		static void CacheClass(std::string_view className, MonoClass* monoClass);
		static void CacheClassMethods(ManagedClass& managedClass);

		static void CacheMethod(MonoClass* monoClass, const std::string& methodName, int32_t parameterCount = 0);
	};
	
}
