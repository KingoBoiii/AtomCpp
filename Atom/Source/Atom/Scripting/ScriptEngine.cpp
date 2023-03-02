#include "ATPCH.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "ScriptUtils.h"
#include "ScriptCache.h"

#include "Atom/Scripting/Assembly/AssemblyUtils.h"

#include "Atom/Core/Application.h"
#include "Atom/Core/FileSystem.h"
#include "Atom/Scene/Scene.h"
#include "Atom/Scene/Entity.h"
#include "Atom/Project/Project.h"

#include "FileWatch.h"

#include <glm/glm.hpp>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tabledefs.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

namespace Atom
{

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		ScriptEngineConfig Config;

		AssemblyInfo* CoreAssemblyInfo = nullptr;
		AssemblyInfo* AppAssemblyInfo = nullptr;

		std::unordered_map<UUID, MonoObject*> EntityManagedInstances;

		filewatch::FileWatch<std::string>* AppAssemblyWatcher = nullptr;
		bool AppAssemblyReloadPending = false;

		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_ScriptEngineData = nullptr;

	void ScriptEngine::Initialize(const ScriptEngineConfig& config)
	{
		s_ScriptEngineData = new ScriptEngineData();
		s_ScriptEngineData->Config = config;

		InitializeMono();

		LoadCoreAssembly();
		ScriptCache::Initialize();

		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterInternalCalls();
	}

	void ScriptEngine::Shutdown()
	{
		ScriptCache::Shutdown();
		ShutdownMono();

		delete s_ScriptEngineData;
	}

	void ScriptEngine::ReloadAssembly()
	{
		UnloadAppAssembly();

		s_ScriptEngineData->EntityManagedInstances.clear();

		LoadCoreAssembly();
		ScriptCache::Initialize();

		LoadAppAssembly();

		ScriptGlue::RegisterComponents();
	}

	bool ScriptEngine::LoadAppAssembly()
	{
		if(!std::filesystem::exists(Project::GetScriptModuleFilepath()))
		{
			AT_CORE_ERROR("[ScriptEngine] Failed to load app assembly! Invalid filepath");
			AT_CORE_ERROR("[ScriptEngine] Filepath = {}", Project::GetScriptModuleFilepath());
			return false;
		}

		s_ScriptEngineData->AppAssemblyInfo = AssemblyUtils::LoadAssembly(Project::GetScriptModuleFilepath(), s_ScriptEngineData->Config.EnableDebugging);
		if(s_ScriptEngineData->AppAssemblyInfo == nullptr)
		{
			AT_CORE_ERROR("[ScriptEngine] Failed to load app assembly!");
			return false;
		}
		
		AT_CORE_INFO("[ScriptEngine] Successfully loaded app assembly from: {0}", Project::GetScriptModuleFilepath());

		ScriptCache::CacheAssemblyClasses(s_ScriptEngineData->AppAssemblyInfo);

		return true;
	}

	void ScriptEngine::UnloadAppAssembly()
	{
		// Call OnDestroy method from here!

		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_ScriptEngineData->AppDomain);

		ScriptCache::Shutdown();
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_ScriptEngineData->SceneContext = scene;

		InstantiateEntityInstances();
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_ScriptEngineData->SceneContext = nullptr;

		s_ScriptEngineData->EntityManagedInstances.clear();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		UUID entityId = entity.GetUUID();
		if(s_ScriptEngineData->EntityManagedInstances.find(entityId) == s_ScriptEngineData->EntityManagedInstances.end())
		{
			AT_CORE_ERROR("[ScriptEngine] Cannot find managed instance for entity: {0}", entity.GetName());
			return;
		}
		
		MonoObject* entityInstance = s_ScriptEngineData->EntityManagedInstances[entity.GetUUID()];
		ScriptCache::InvokeEntityStart(entityInstance);
	}

	void ScriptEngine::OnDestroyEntity(Entity entity)
	{
		const auto& script = entity.GetComponent<Component::Script>();

		UUID entityId = entity.GetUUID();
		if(s_ScriptEngineData->EntityManagedInstances.find(entityId) == s_ScriptEngineData->EntityManagedInstances.end())
		{
			AT_CORE_ERROR("[ScriptEngine] Cannot find managed instance for entity: {0}", entity.GetName());
			return;
		}

		MonoObject* entityInstance = s_ScriptEngineData->EntityManagedInstances[entity.GetUUID()];
		ScriptCache::InvokeEntityDestroy(entityInstance);

		s_ScriptEngineData->EntityManagedInstances.erase(entity.GetUUID());
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, float deltaTime)
	{
		UUID entityId = entity.GetUUID();
		if(s_ScriptEngineData->EntityManagedInstances.find(entityId) == s_ScriptEngineData->EntityManagedInstances.end())
		{
			AT_CORE_ERROR("[ScriptEngine] Cannot find managed instance for entity: {0}", entity.GetName());
			return;
		}

		MonoObject* entityInstance = s_ScriptEngineData->EntityManagedInstances[entity.GetUUID()];
		ScriptCache::InvokeEntityUpdate(entityInstance, deltaTime);
	}

	void ScriptEngine::InvokeOnCollection2DEnter(Entity entity, Entity other)
	{
		if(!entity.HasComponent<Component::Script>())
		{
			return;
		}

		UUID entityId = entity.GetUUID();
		if(s_ScriptEngineData->EntityManagedInstances.find(entityId) == s_ScriptEngineData->EntityManagedInstances.end())
		{
			AT_CORE_ERROR("[ScriptEngine] Cannot find managed instance for entity: {0}", entity.GetName());
			return;
		}

		MonoObject* entityInstance = s_ScriptEngineData->EntityManagedInstances[entity.GetUUID()];
		ScriptCache::InvokeEntityOnCollision2DEnter(entityInstance, other.GetUUID());
	}

	void ScriptEngine::InvokeOnCollection2DExit(Entity entity, Entity other)
	{
		if(!entity.HasComponent<Component::Script>())
		{
			return;
		}

		UUID entityId = entity.GetUUID();
		if(s_ScriptEngineData->EntityManagedInstances.find(entityId) == s_ScriptEngineData->EntityManagedInstances.end())
		{
			AT_CORE_ERROR("[ScriptEngine] Cannot find managed instance for entity: {0}", entity.GetName());
			return;
		}

		MonoObject* entityInstance = s_ScriptEngineData->EntityManagedInstances[entity.GetUUID()];
		ScriptCache::InvokeEntityOnCollision2DExit(entityInstance, other.GetUUID());
	}

	static void OnAppAssemblyFileSystemEvent(const std::string& filepath, filewatch::Event changeType)
	{
		if(!s_ScriptEngineData->AppAssemblyReloadPending && changeType == filewatch::Event::modified)
		{
			s_ScriptEngineData->AppAssemblyReloadPending = true;

			//using namespace std::chrono_literals;
			//std::this_thread::sleep_for(500ms);

			// Reload assembly
			// Add reload to main thread queue
			Application::Get().SubmitToMainThread([]()
			{
				delete s_ScriptEngineData->AppAssemblyWatcher;
			s_ScriptEngineData->AppAssemblyWatcher = nullptr;

			AT_CORE_INFO("Reloading Assembly!");

			ScriptEngine::ReloadAssembly();
			});
		}
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID entityId)
	{
		auto iterator = s_ScriptEngineData->EntityManagedInstances.find(entityId);
		if(iterator != s_ScriptEngineData->EntityManagedInstances.end())
		{
			return nullptr;
		}

		return iterator->second;
	}

	AssemblyInfo* ScriptEngine::GetCoreAssemblyInfo()
	{
		return s_ScriptEngineData->CoreAssemblyInfo;
	}

	AssemblyInfo* ScriptEngine::GetAppAssemblyInfo()
	{
		return s_ScriptEngineData->AppAssemblyInfo;
	}

	void ScriptEngine::InitializeMono()
	{
		mono_set_assemblies_path("mono/lib");

		if(s_ScriptEngineData->Config.EnableDebugging)
		{
			const char* argv[2] = {
				"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.txt",
				"--soft-breakpoints"
			};

			mono_jit_parse_options(2, (char**)argv);
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}

		MonoDomain* rootDomain = mono_jit_init("AtomJITRuntime");
		AT_CORE_ASSERT(rootDomain);

		s_ScriptEngineData->RootDomain = rootDomain;

		if(s_ScriptEngineData->Config.EnableDebugging)
		{
			mono_debug_domain_create(s_ScriptEngineData->RootDomain);
		}

		mono_thread_set_main(mono_thread_current());
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_ScriptEngineData->AppDomain);
		s_ScriptEngineData->AppDomain = nullptr;

		mono_jit_cleanup(s_ScriptEngineData->RootDomain);
		s_ScriptEngineData->RootDomain = nullptr;
	}

	void ScriptEngine::InstantiateEntityInstances()
	{
		// Instantiate every managed entity class
		auto view = s_ScriptEngineData->SceneContext->GetAllEntitiesWith<Component::Identifier, Component::Script>();
		for(const auto& e : view)
		{
			auto [identifier, script] = view.get<Component::Identifier, Component::Script>(e);

			ManagedClass* managedEntityClass = AT_CORE_CLASS("Entity");
			ManagedClass* managedClass = AT_CACHED_ENTITY_CLASS(script.ClassName);
			if(!managedEntityClass || !managedClass)
			{
				AT_CORE_ERROR("[ScriptEngine] Failed to find managed class for entity: {0}", identifier.Name);
				continue;
			}

			MonoObject* entityInstance = ScriptUtils::InstantiateManagedClass(managedClass);

			void* param = &identifier.ID;
			ScriptUtils::InvokeManagedMethod(AT_CACHED_METHOD(managedEntityClass, ".ctor"), entityInstance, &param);

			// Copy values from Editor to Runtime
			for(const auto& [name, field] : managedClass->GetFields())
			{
				mono_field_set_value(entityInstance, field.GetRawClassField(), (void*)field.m_FieldValueBuffer.Data);
			}

			s_ScriptEngineData->EntityManagedInstances[identifier.ID] = entityInstance;
		}
	}

	void* ScriptEngine::GetEntityInstanceFieldValueInternal(Entity entity, ManagedClassField* managedClassField)
	{
		UUID entityId = entity.GetUUID();
		if(s_ScriptEngineData->EntityManagedInstances.find(entityId) == s_ScriptEngineData->EntityManagedInstances.end())
		{
			AT_CORE_ERROR("[ScriptEngine] Cannot find managed instance for entity: {0}", entity.GetName());
			return nullptr;
		}

		MonoObject* entityInstance = s_ScriptEngineData->EntityManagedInstances[entity.GetUUID()];

		uint8_t* buffer = new uint8_t[16];
		mono_field_get_value(entityInstance, managedClassField->GetRawClassField(), buffer);
		return buffer;
	}

	void ScriptEngine::SetEntityInstanceFieldValueInternal(Entity entity, ManagedClassField* managedClassField, const void* buffer)
	{
		UUID entityId = entity.GetUUID();
		if(s_ScriptEngineData->EntityManagedInstances.find(entityId) == s_ScriptEngineData->EntityManagedInstances.end())
		{
			AT_CORE_ERROR("[ScriptEngine] Cannot find managed instance for entity: {0}", entity.GetName());
			return;
		}

		MonoObject* entityInstance = s_ScriptEngineData->EntityManagedInstances[entity.GetUUID()];

		mono_field_set_value(entityInstance, managedClassField->GetRawClassField(), (void*)buffer);
	}

	bool ScriptEngine::LoadCoreAssembly()
	{
		if(!std::filesystem::exists(s_ScriptEngineData->Config.CoreAssemblyPath))
		{
			return false;
		}

		s_ScriptEngineData->AppDomain = mono_domain_create_appdomain("AtomScriptRuntime", nullptr);
		mono_domain_set(s_ScriptEngineData->AppDomain, true);

		s_ScriptEngineData->CoreAssemblyInfo = AssemblyUtils::LoadAssembly(s_ScriptEngineData->Config.CoreAssemblyPath, s_ScriptEngineData->Config.EnableDebugging);
		if(s_ScriptEngineData->CoreAssemblyInfo == nullptr)
		{
			AT_CORE_ERROR("[ScriptEngine] Failed to load core assembly!");
			return false;
		}

		AT_CORE_INFO("[ScriptEngine] Successfully loaded core assembly from: {0}", s_ScriptEngineData->Config.CoreAssemblyPath);

		return true;
	}

	MonoDomain* ScriptEngine::GetAppDomain()
	{
		return s_ScriptEngineData->AppDomain;
	}

	void ScriptEngine::SetSceneContext(Scene* scene)
	{
		s_ScriptEngineData->SceneContext = scene;
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_ScriptEngineData->SceneContext;
	}

}