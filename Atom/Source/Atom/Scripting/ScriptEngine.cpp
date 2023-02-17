#include "ATPCH.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include "Atom/Scene/Scene.h"
#include "Atom/Scene/Entity.h"

#include <glm/glm.hpp>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace Atom
{

	namespace Utils
	{

		char* ReadBytes(const std::string& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if(!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if(size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if(status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for(int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				AT_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass EntityClass;

		std::unordered_map<std::string, ScriptClass*> EntityClasses;
		std::unordered_map<UUID, ScriptInstance*> EntityInstances;
	};

	static ScriptEngineData* s_ScriptEngineData = nullptr;

	ScriptEngine* ScriptEngine::s_Instance = nullptr;

	ScriptEngine::ScriptEngine(Scene* scene)
		: m_Scene(scene)
	{
		AT_CORE_ASSERT(!s_Instance, "ScriptEngine already exists!");
		s_Instance = this;

		s_ScriptEngineData = new ScriptEngineData();
	}

	ScriptEngine::~ScriptEngine()
	{
		Shutdown();

		delete s_ScriptEngineData;
	}

	void ScriptEngine::Initialize()
	{
		InitializeMono();
		LoadAssembly("Resources/Scripts/Atom.Core.dll");
		LoadAssemblyClasses(s_ScriptEngineData->CoreAssembly, s_ScriptEngineData->CoreAssemblyImage);

		ScriptGlue::RegisterInternalCalls();

		s_ScriptEngineData->EntityClass = ScriptClass("Atom", "EntityBase");

#if 0
		s_ScriptEngineData->EntityClass = ScriptClass(s_ScriptEngineData->CoreAssemblyImage, "Atom", "Main");

		MonoObject* instance = s_ScriptEngineData->EntityClass.Instantiate();
		MonoMethod* printMessageMethod = s_ScriptEngineData->EntityClass.GetMethod("PrintMessage");
		s_ScriptEngineData->EntityClass.InvokeMethod(instance, printMessageMethod);

		MonoMethod* printCustomMessageMethod = s_ScriptEngineData->EntityClass.GetMethod("PrintCustomMessage", 1);
		MonoString* str = mono_string_new(s_ScriptEngineData->AppDomain, "Hello world from C++");
		s_ScriptEngineData->EntityClass.InvokeMethod(instance, printCustomMessageMethod, (void**)&str);
#endif


#if 0
		MonoClass* monoClass = mono_class_from_name(s_ScriptEngineData->CoreAssemblyImage, "Atom", "Main");
		MonoObject* instance = mono_object_new(s_ScriptEngineData->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		MonoMethod* printMessageMethod = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageMethod, instance, nullptr, nullptr);

		MonoString* str = mono_string_new(s_ScriptEngineData->AppDomain, "Hello world from C++");
		MonoMethod* printCustomMessageMethod = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		mono_runtime_invoke(printCustomMessageMethod, instance, (void**)&str, nullptr);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
	}

	void ScriptEngine::OnRuntimeStart()
	{
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_ScriptEngineData->EntityInstances.clear();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& script = entity.GetComponent<Component::Script>();
		if(EntityClassExists(script.ClassName))
		{
			ScriptClass* scriptClass = s_ScriptEngineData->EntityClasses[script.ClassName];

			ScriptInstance* scriptInstance = new ScriptInstance(scriptClass, entity);

			s_ScriptEngineData->EntityInstances[entity.GetUUID()] = scriptInstance;

			scriptInstance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnDestroyEntity(Entity entity)
	{
		UUID entityUUID = entity.GetUUID();
		AT_CORE_ASSERT(s_ScriptEngineData->EntityInstances.find(entityUUID) != s_ScriptEngineData->EntityInstances.end());

		ScriptInstance* scriptInstance = s_ScriptEngineData->EntityInstances[entityUUID];
		scriptInstance->InvokeOnDestroy();

		delete scriptInstance;
		s_ScriptEngineData->EntityInstances.erase(entity.GetUUID());
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, float deltaTime)
	{
		UUID entityUUID = entity.GetUUID();
		AT_CORE_ASSERT(s_ScriptEngineData->EntityInstances.find(entityUUID) != s_ScriptEngineData->EntityInstances.end());

		ScriptInstance* scriptInstance = s_ScriptEngineData->EntityInstances[entityUUID];
		scriptInstance->InvokeOnUpdate(deltaTime);
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_ScriptEngineData->AppDomain = mono_domain_create_appdomain("AtomScriptRuntime", nullptr);
		mono_domain_set(s_ScriptEngineData->AppDomain, true);

		s_ScriptEngineData->CoreAssembly = Utils::LoadCSharpAssembly("Resources/Scripts/Atom.Core.dll");
		s_ScriptEngineData->CoreAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->CoreAssembly);
		//Utils::PrintAssemblyTypes(s_ScriptEngineData->CoreAssembly);
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullName)
	{
		return s_ScriptEngineData->EntityClasses.find(fullName) != s_ScriptEngineData->EntityClasses.end();
	}

	std::unordered_map<std::string, ScriptClass*> ScriptEngine::GetEntityClasses()
	{
		return s_ScriptEngineData->EntityClasses;
	}

	void ScriptEngine::InitializeMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("AtomJITRuntime");
		AT_CORE_ASSERT(rootDomain);

		s_ScriptEngineData->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		s_ScriptEngineData->AppDomain = nullptr;

		mono_jit_cleanup(s_ScriptEngineData->RootDomain);
		s_ScriptEngineData->RootDomain = nullptr;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_ScriptEngineData->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::LoadAssemblyClasses(MonoAssembly* assembly, MonoImage* image)
	{
		s_ScriptEngineData->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityBaseClass = mono_class_from_name(image, "Atom", "EntityBase");

		for(int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if(strlen(nameSpace) != 0)
			{
				fullName = fmt::format("{}.{}", nameSpace, name);
			}
			else
			{
				fullName = name;
			}

			MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);
			if(monoClass == nullptr)
			{
				continue;
			}

			if(monoClass == entityBaseClass)
			{
				continue;
			}

			bool isComponent = mono_class_is_subclass_of(monoClass, entityBaseClass, false);
			if(isComponent)
			{
				s_ScriptEngineData->EntityClasses[fullName] = new ScriptClass(nameSpace, name);
			}
		}
	}

	ScriptClass::ScriptClass(const std::string& classNameSpace, const std::string& className)
		: m_ClassNamespace(classNameSpace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(s_ScriptEngineData->CoreAssemblyImage, classNameSpace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate() const
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, int parameterCount) const
	{
		return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* monoMethod, void** parameters)
	{
		return mono_runtime_invoke(monoMethod, instance, parameters, nullptr);
	}

	ScriptInstance::ScriptInstance(ScriptClass* scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = m_ScriptClass->Instantiate();

		m_Constructor = s_ScriptEngineData->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate");
		m_OnDestroyMethod = m_ScriptClass->GetMethod("OnDestroy");
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);

		{
			void* param = &entity.GetUUID();

			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
		}
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

}