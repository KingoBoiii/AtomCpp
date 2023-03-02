#include "ATPCH.h"
#include "ScriptCache.h"
#include "ScriptEngine.h"
#include "ScriptUtils.h"
#include "Atom/Scripting/Managed/ManagedMethodThunk.h"

#include "Atom/Core/Hash.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tabledefs.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/appdomain.h>

namespace Atom
{

	struct Cache
	{
		std::unordered_map<std::string, ManagedClass> EntityClasses;
		std::unordered_map<std::string, ManagedClass> Classes;
		//std::unordered_map<uint32_t, ManagedClass> Classes;

		std::unordered_map<std::string, ManagedMethod> Methods;
	};

	struct MethodThunkCache
	{
		ManagedMethodThunk<> Entity_StartMethod;
		ManagedMethodThunk<> Entity_DestroyMethod;
		ManagedMethodThunk<float> Entity_UpdateMethod;
	};

	static Cache* s_Cache = nullptr;
	static MethodThunkCache* s_MethodThunkCache = nullptr;

	void ScriptCache::Initialize()
	{
		s_Cache = new Cache();
		s_MethodThunkCache = new MethodThunkCache();

		CacheCoreClasses();

#define REGISTER_METHOD_THUNK(var, method) s_MethodThunkCache->var.RegisterMethodThunk(method);

		REGISTER_METHOD_THUNK(Entity_StartMethod, AT_CACHED_METHOD(AT_CACHED_CLASS("Atom.Entity"), "Start"));
		REGISTER_METHOD_THUNK(Entity_DestroyMethod, AT_CACHED_METHOD(AT_CACHED_CLASS("Atom.Entity"), "Destroy"));
		REGISTER_METHOD_THUNK(Entity_UpdateMethod, AT_CACHED_METHOD(AT_CACHED_CLASS("Atom.Entity"), "Update"));

#undef REGISTER_METHOD_THUNK
	}

	void ScriptCache::Shutdown()
	{
		ClearCache();

		delete s_Cache;
		s_Cache = nullptr;

		delete s_MethodThunkCache;
		s_MethodThunkCache = nullptr;
	}

	void ScriptCache::ClearCache()
	{
		if(s_Cache == nullptr)
		{
			return;
		}

		s_Cache->EntityClasses.clear();
		s_Cache->Classes.clear();
		s_Cache->Methods.clear();
	}

	void ScriptCache::CacheCoreClasses()
	{
		if(s_Cache == nullptr)
		{
			return;
		}

#define CACHE_CORELIB_CLASS(name) CacheClass("System." ##name, mono_class_from_name(mono_get_corlib(), "System", name))

		CACHE_CORELIB_CLASS("Object");
		CACHE_CORELIB_CLASS("ValueType");
		CACHE_CORELIB_CLASS("Boolean");
		CACHE_CORELIB_CLASS("SByte");
		CACHE_CORELIB_CLASS("Int16");
		CACHE_CORELIB_CLASS("Int32");
		CACHE_CORELIB_CLASS("Int64");
		CACHE_CORELIB_CLASS("Byte");
		CACHE_CORELIB_CLASS("UInt16");
		CACHE_CORELIB_CLASS("UInt32");
		CACHE_CORELIB_CLASS("UInt64");
		CACHE_CORELIB_CLASS("Single");
		CACHE_CORELIB_CLASS("Double");
		CACHE_CORELIB_CLASS("Char");
		CACHE_CORELIB_CLASS("String");

		auto d = ScriptEngine::GetCoreAssemblyInfo();

#define CACHE_ATOM_CORE_CLASS(name) CacheClass("Atom." ##name, mono_class_from_name(ScriptEngine::GetCoreAssemblyInfo()->AssemblyImage, "Atom", name))

		CACHE_ATOM_CORE_CLASS("VisibleInEditorAttribute");
		CACHE_ATOM_CORE_CLASS("HiddenFromEditorAttribute");

		//CACHE_ATOM_CORE_CLASS("Vector2");
		//CACHE_ATOM_CORE_CLASS("Vector3");
		//CACHE_ATOM_CORE_CLASS("Vector4");
		CACHE_ATOM_CORE_CLASS("Entity");

//#define CACHE_ATOM_ENTITY_METHOD(name, parameterCount) CacheMethod(AT_CACHED_CLASS_RAW("Atom.Entity"), name, parameterCount);
//		
//		CACHE_ATOM_ENTITY_METHOD(".ctor", 1);
//		CACHE_ATOM_ENTITY_METHOD("Start", 0);
//		CACHE_ATOM_ENTITY_METHOD("Destroy", 0);
//		CACHE_ATOM_ENTITY_METHOD("Update", 1);
	}

	void ScriptCache::CacheAssemblyClasses(AssemblyInfo* assemblyInfo)
	{
		s_Cache->EntityClasses.clear();

		const MonoTableInfo* monoTableInfo = mono_image_get_table_info(assemblyInfo->AssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t tableRowCount = mono_table_info_get_rows(monoTableInfo);

		// starting at index 1, to skip .<Module>
		for(int32_t i = 1; i < tableRowCount; i++)
		{
			MonoClass* monoClass = mono_class_get(assemblyInfo->AssemblyImage, (i + 1) | MONO_TOKEN_TYPE_DEF);

			if(!mono_class_is_subclass_of(monoClass, AT_CACHED_CLASS_RAW("Atom.Entity"), false))
			{
				continue;
			}

			ManagedClass managedClass(monoClass);

			CacheClassMethods(managedClass);

			s_Cache->EntityClasses[managedClass.GetFullName()] = managedClass;
		}
	}

	ManagedMethod* ScriptCache::GetManagedMethodByName(ManagedClass* managedClass, const std::string& methodName)
	{
		if(s_Cache == nullptr)
		{
			return nullptr;
		}

		std::string fullMethodName = managedClass->GetFullName() + ":" + methodName;

		auto iterator = s_Cache->Methods.find(fullMethodName);
		if(iterator == s_Cache->Methods.end())
		{
			return nullptr;
		}

		return &iterator->second;
	}

	ManagedClass* ScriptCache::GetManagedEntityClassByName(const std::string& className)
	{
		if(s_Cache == nullptr)
		{
			return nullptr;
		}

		auto iterator = s_Cache->EntityClasses.find(className);
		if(iterator == s_Cache->EntityClasses.end())
		{
			return nullptr;
		}

		return &iterator->second;
	}

	ManagedClass* ScriptCache::GetManagedClassByName(const std::string& className)
	{
		if(s_Cache == nullptr)
		{
			return nullptr;
		}

		auto iterator = s_Cache->Classes.find(className);
		if(iterator == s_Cache->Classes.end())
		{
			return nullptr;
		}

		return &iterator->second;
	}

	void ScriptCache::InvokeEntityStart(MonoObject* instance)
	{
		MonoException* exception = nullptr;
		s_MethodThunkCache->Entity_StartMethod.Invoke(instance, &exception);
		ScriptUtils::HandleException((MonoObject*)exception);
	}

	void ScriptCache::InvokeEntityDestroy(MonoObject* instance)
	{
		MonoException* exception = nullptr;
		s_MethodThunkCache->Entity_DestroyMethod.Invoke(instance, &exception);
		ScriptUtils::HandleException((MonoObject*)exception);
	}

	void ScriptCache::InvokeEntityUpdate(MonoObject* instance, float deltaTime)
	{
		MonoException* exception = nullptr;
		s_MethodThunkCache->Entity_UpdateMethod.Invoke(instance, deltaTime, &exception);
		ScriptUtils::HandleException((MonoObject*)exception);
	}

	void ScriptCache::CacheClass(std::string_view className, MonoClass* monoClass)
	{
		ManagedClass managedClass(monoClass);

		CacheClassMethods(managedClass);

		s_Cache->Classes[managedClass.GetFullName()] = managedClass;
	}

	void ScriptCache::CacheClassMethods(ManagedClass& managedClass)
	{
		void* iterator = 0;
		while(MonoMethod* monoMethod = mono_class_get_methods(managedClass.GetRawClass(), &iterator))
		{
			ManagedMethod managedMethod(monoMethod);

			s_Cache->Methods[managedMethod.GetFullName()] = managedMethod;
		}
	}

	void ScriptCache::CacheMethod(MonoClass* monoClass, const std::string& methodName, int32_t parameterCount)
	{
#if 0
		MonoMethod* monoMethod = mono_class_get_method_from_name(monoClass, methodName.data(), parameterCount);
		if(monoMethod == nullptr)
		{
			AT_CORE_ERROR("Failed to find method {} in class {}", methodName, mono_class_get_name(monoClass));
			return;
		}

		ManagedMethod& managedMethod = s_Cache->Methods[methodName];
		managedMethod.Method = monoMethod;
		managedMethod.FullName = std::string(methodName);
		managedMethod.ParameterCount = parameterCount;
#endif
	}

}