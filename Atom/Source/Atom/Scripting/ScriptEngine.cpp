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

		std::unordered_map<UUID, ScriptInstance*> EntityInstances;

		std::unordered_map<UUID, MonoObject*> EntityManagedInstances;

		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		filewatch::FileWatch<std::string>* AppAssemblyWatcher = nullptr;
		bool AppAssemblyReloadPending = false;

		// TODO: Move to ScriptEngineConfig
		bool EnableDebugging = true;

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
		ScriptCache::CacheCoreClasses();

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

		s_ScriptEngineData->AppAssemblyInfo = AssemblyUtils::LoadAssembly(Project::GetScriptModuleFilepath(), s_ScriptEngineData->EnableDebugging);
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
		//s_ScriptEngineData->EntityInstances.clear();
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
		
#if 0
		if(EntityClassExists(script.ClassName))
		{
			ScriptClass* scriptClass = s_ScriptEngineData->EntityClasses[script.ClassName];

			ScriptInstance* scriptInstance = new ScriptInstance(scriptClass, entity);

			UUID entityId = entity.GetUUID();
			
			s_ScriptEngineData->EntityInstances[entityId] = scriptInstance;

			// Copy field values
			if(s_ScriptEngineData->EntityScriptFields.find(entityId) != s_ScriptEngineData->EntityScriptFields.end())
			{
				const ScriptFieldMap& fieldMap = s_ScriptEngineData->EntityScriptFields.at(entityId);
				for(const auto& [name, fieldInstance] : fieldMap)
				{
					scriptInstance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
				}
			}

			scriptInstance->InvokeOnCreate();
		}
#endif
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

#if 0
	void ScriptEngine::InvokeOnCollection2DEnter(Entity entity, Entity other)
	{
		if(!entity.HasComponent<Component::Script>())
		{
			return;
		}

		UUID entityUUID = entity.GetUUID();
		AT_CORE_ASSERT(s_ScriptEngineData->EntityInstances.find(entityUUID) != s_ScriptEngineData->EntityInstances.end());

		ScriptInstance* scriptInstance = s_ScriptEngineData->EntityInstances[entityUUID];
		scriptInstance->InvokeOnCollision2DEnter(other);
	}

	void ScriptEngine::InvokeOnCollection2DExit(Entity entity, Entity other)
	{
		if(!entity.HasComponent<Component::Script>())
		{
			return;
		}

		UUID entityUUID = entity.GetUUID();
		AT_CORE_ASSERT(s_ScriptEngineData->EntityInstances.find(entityUUID) != s_ScriptEngineData->EntityInstances.end());

		ScriptInstance* scriptInstance = s_ScriptEngineData->EntityInstances[entityUUID];
		scriptInstance->InvokeOnCollision2DExit(other);
	}
#endif

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

#if 0
	bool ScriptEngine::EntityClassExists(const std::string& fullName)
	{
		return s_ScriptEngineData->EntityClasses.find(fullName) != s_ScriptEngineData->EntityClasses.end();
	}

	ScriptClass* ScriptEngine::GetEntityClass(const std::string& name)
	{
		if(s_ScriptEngineData->EntityClasses.find(name) == s_ScriptEngineData->EntityClasses.end())
		{
			return nullptr;
		}

		return s_ScriptEngineData->EntityClasses.at(name);
	}

	std::unordered_map<std::string, ScriptClass*> ScriptEngine::GetEntityClasses()
	{
		return s_ScriptEngineData->EntityClasses;
	}
#endif
	
	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		AT_CORE_ASSERT(entity);

		UUID entityId = entity.GetUUID();
		//AT_CORE_ASSERT(s_ScriptEngineData->EntityScriptFields.find(entityId) != s_ScriptEngineData->EntityScriptFields.end());

		return s_ScriptEngineData->EntityScriptFields[entityId];
	}

	ScriptInstance* ScriptEngine::GetEntityScriptInstance(UUID entityId)
	{
		auto it = s_ScriptEngineData->EntityInstances.find(entityId);
		if(it == s_ScriptEngineData->EntityInstances.end())
		{
			return nullptr;
		}

		return it->second;
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID entityId)
	{
		AT_CORE_ASSERT(s_ScriptEngineData->EntityInstances.find(entityId) != s_ScriptEngineData->EntityInstances.end());

		ScriptInstance* scriptInstance = s_ScriptEngineData->EntityInstances.at(entityId);
		return scriptInstance->GetManagedObject();
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

		if(s_ScriptEngineData->EnableDebugging)
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

		if(s_ScriptEngineData->EnableDebugging)
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

			s_ScriptEngineData->EntityManagedInstances[identifier.ID] = entityInstance;
		}
	}

	bool ScriptEngine::LoadCoreAssembly()
	{
		if(!std::filesystem::exists(s_ScriptEngineData->Config.CoreAssemblyPath))
		{
			return false;
		}

		s_ScriptEngineData->AppDomain = mono_domain_create_appdomain("AtomScriptRuntime", nullptr);
		mono_domain_set(s_ScriptEngineData->AppDomain, true);

		s_ScriptEngineData->CoreAssemblyInfo = AssemblyUtils::LoadAssembly(s_ScriptEngineData->Config.CoreAssemblyPath, s_ScriptEngineData->EnableDebugging);
		if(s_ScriptEngineData->CoreAssemblyInfo == nullptr)
		{
			AT_CORE_ERROR("[ScriptEngine] Failed to load core assembly!");
			return false;
		}

		AT_CORE_INFO("[ScriptEngine] Successfully loaded core assembly from: {0}", s_ScriptEngineData->Config.CoreAssemblyPath);

		return true;
	}
	
	static MonoObject* InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_ScriptEngineData->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
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

	ScriptClass::ScriptClass(const std::string& classNameSpace, const std::string& className, bool isCore)
		: m_ClassNamespace(classNameSpace), m_ClassName(className)
	{
		MonoImage* image = isCore ? s_ScriptEngineData->CoreAssemblyInfo->AssemblyImage : s_ScriptEngineData->AppAssemblyInfo->AssemblyImage;
		m_MonoClass = mono_class_from_name(image, classNameSpace.c_str(), className.c_str());
		//m_MonoClass = mono_class_from_name(isCore ? s_ScriptEngineData->CoreAssemblyImage : s_ScriptEngineData->AppAssemblyImage, classNameSpace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate() const
	{
		return InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, int parameterCount) const
	{
		return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* monoMethod, void** parameters)
	{
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(monoMethod, instance, parameters, &exception);
	}

	ScriptInstance::ScriptInstance(ScriptClass* scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
#if 0
		m_Instance = m_ScriptClass->Instantiate();

		m_Constructor = s_ScriptEngineData->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate");
		m_OnDestroyMethod = m_ScriptClass->GetMethod("OnDestroy");
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);

		m_OnCollision2DEnterMethod = s_ScriptEngineData->EntityClass.GetMethod("OnCollision2DEnter_Internal", 1);
		m_OnCollision2DExitMethod = s_ScriptEngineData->EntityClass.GetMethod("OnCollision2DExit_Internal", 1);

		{
			void* param = &entity.GetUUID();

			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
		}
#endif
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if(!m_OnCreateMethod)
		{
			return;
		}

		m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnDestroy()
	{
		if(!m_OnDestroyMethod)
		{
			return;
		}

		m_ScriptClass->InvokeMethod(m_Instance, m_OnDestroyMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float deltaTime)
	{
		if(!m_OnUpdateMethod)
		{
			return;
		}

		void* param = &deltaTime;
		m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
	}

#if 0
	void ScriptInstance::InvokeOnCollision2DEnter(Entity other)
	{
		if(!m_OnCollision2DEnterMethod)
		{
			return;
		}

		void* param = &other.GetUUID();
		m_ScriptClass->InvokeMethod(m_Instance, m_OnCollision2DEnterMethod, &param);
	}

	void ScriptInstance::InvokeOnCollision2DExit(Entity other)
	{
		if(!m_OnCollision2DExitMethod)
		{
			return;
		}

		void* param = &other.GetUUID();
		m_ScriptClass->InvokeMethod(m_Instance, m_OnCollision2DExitMethod, &param);
	}
#endif

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetFields();

		auto it = fields.find(name);

		if(it == fields.end())
		{
			return false;
		}

		const ScriptField& field = it->second;

		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->GetFields();

		auto it = fields.find(name);

		if(it == fields.end())
		{
			return false;
		}

		const ScriptField& field = it->second;

		mono_field_set_value(m_Instance, field.ClassField, (void*)value);
		return true;
	}

}