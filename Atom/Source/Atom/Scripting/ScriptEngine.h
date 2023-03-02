#pragma once
#include "Assembly/AssemblyInfo.h"
#include "Assembly/AssemblyMetadata.h"
#include "Managed/ManagedClassField.h"

#include "Atom/Core/UUID.h"

#include <map>
#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoClassField MonoClassField;
}

#define ATOM_CORE_ASSEMBLY_INDEX 0
#define ATOM_APP_ASSEMBLY_INDEX ATOM_CORE_ASSEMBLY_INDEX + 1
#define ATOM_MAX_ASSEMBLIES (size_t)2

namespace Atom
{

	struct ScriptEngineConfig
	{
		std::filesystem::path CoreAssemblyPath;

		bool EnableDebugging;
	};

	class Scene;
	class Entity;

	class ATOM_API ScriptEngine
	{
	public:
		static void Initialize(const ScriptEngineConfig& config);
		static void Shutdown();
		static void ReloadAssembly();

		static bool LoadAppAssembly();
		static void UnloadAppAssembly();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void OnCreateEntity(Entity entity);
		static void OnDestroyEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float deltaTime);

		static void InvokeOnCollection2DEnter(Entity entity, Entity other);
		static void InvokeOnCollection2DExit(Entity entity, Entity other);
		
		static void SetSceneContext(Scene* scene);
		static Scene* GetSceneContext();

		static MonoObject* GetManagedInstance(UUID entityId);

		static AssemblyInfo* GetCoreAssemblyInfo();
		static AssemblyInfo* GetAppAssemblyInfo();
		static MonoDomain* GetAppDomain();

		template<typename T>
		static T GetEntityInstanceFieldValue(Entity entity, ManagedClassField* managedClassField)
		{
			void* buffer = GetEntityInstanceFieldValueInternal(entity, managedClassField);
			return *(T*)buffer;
		}

		template<typename T>
		static void SetEntityInstanceFieldValue(Entity entity, ManagedClassField* managedClassField, const T& value)
		{
			SetEntityInstanceFieldValueInternal(entity, managedClassField, &value);
		}
	private:
		static void InitializeMono();
		static void ShutdownMono();

		static void InstantiateEntityInstances();
		static void* GetEntityInstanceFieldValueInternal(Entity entity, ManagedClassField* managedClassField);
		static void SetEntityInstanceFieldValueInternal(Entity entity, ManagedClassField* managedClassField, const void* buffer);

		static bool LoadCoreAssembly();

		friend class ScriptClass;
		friend class ScriptGlue;
	};
	
}
