#include "ATPCH.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include "Atom/Scene/Scene.h"
#include "Atom/Scene/Entity.h"

#include <glm/glm.hpp>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tabledefs.h>

namespace Atom
{

	// TODO: Add unsigned types (and missing types)
	using ScriptFieldTypeMap = std::unordered_map<std::string, ScriptFieldType>;
	static ScriptFieldTypeMap s_ScriptFieldTypeMap = {
		{ "System.Boolean",		ScriptFieldType::Bool },
		{ "System.Char",		ScriptFieldType::Char },
		{ "System.String",		ScriptFieldType::String },
		{ "System.Single",		ScriptFieldType::Float },
		{ "System.Double",		ScriptFieldType::Double },
		{ "System.Byte",		ScriptFieldType::Byte },
		{ "System.Int16",		ScriptFieldType::Short },
		{ "System.Int32",		ScriptFieldType::Int },
		{ "System.Int64",		ScriptFieldType::Long },
		{ "Atom.Vector2",		ScriptFieldType::Vector2 },
		{ "Atom.Vector3",		ScriptFieldType::Vector3 },
		{ "Atom.Vector4",		ScriptFieldType::Vector4 },
		{ "Atom.EntityBase",	ScriptFieldType::Entity }
	};

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

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if(it == s_ScriptFieldTypeMap.end())
			{
				return ScriptFieldType::None;
			}

			return it->second;
		}

		const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
		{
			switch(fieldType)
			{
				case ScriptFieldType::Bool: return "Bool";
				case ScriptFieldType::Char: return "Char";
				case ScriptFieldType::String: return "String";
				case ScriptFieldType::Float: return "Float";
				case ScriptFieldType::Double: return "Double";
				case ScriptFieldType::Byte: return "Byte";
				case ScriptFieldType::Short: return "Short";
				case ScriptFieldType::Int: return "Int";
				case ScriptFieldType::Long: return "Long";
				case ScriptFieldType::Vector2: return "Vector2";
				case ScriptFieldType::Vector3: return "Vector3";
				case ScriptFieldType::Vector4: return "Vector4";
				case ScriptFieldType::Entity: return "Entity";
				case ScriptFieldType::None:
				default: return "Invalid";
			}
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		ScriptClass EntityClass;

		Scene* SceneContext;

		std::unordered_map<std::string, ScriptClass*> EntityClasses;
		std::unordered_map<UUID, ScriptInstance*> EntityInstances;
	};

	static ScriptEngineData* s_ScriptEngineData = nullptr;

	void ScriptEngine::Initialize()
	{
		s_ScriptEngineData = new ScriptEngineData();

		InitializeMono();
		LoadAssembly("Resources/Scripts/Atom.Core.dll");
		LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterInternalCalls();

		s_ScriptEngineData->EntityClass = ScriptClass("Atom", "Entity", true);
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();

		delete s_ScriptEngineData;
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_ScriptEngineData->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_ScriptEngineData->SceneContext = nullptr;

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
	
	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_ScriptEngineData->AppDomain = mono_domain_create_appdomain("AtomScriptRuntime", nullptr);
		mono_domain_set(s_ScriptEngineData->AppDomain, true);

		s_ScriptEngineData->CoreAssembly = Utils::LoadCSharpAssembly(filepath.string());
		s_ScriptEngineData->CoreAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->CoreAssembly);
		//Utils::PrintAssemblyTypes(s_ScriptEngineData->CoreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_ScriptEngineData->AppAssembly = Utils::LoadCSharpAssembly(filepath.string());
		s_ScriptEngineData->AppAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->AppAssembly);
		//Utils::PrintAssemblyTypes(s_ScriptEngineData->AppAssembly);
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullName)
	{
		return s_ScriptEngineData->EntityClasses.find(fullName) != s_ScriptEngineData->EntityClasses.end();
	}

	std::unordered_map<std::string, ScriptClass*> ScriptEngine::GetEntityClasses()
	{
		return s_ScriptEngineData->EntityClasses;
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

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_ScriptEngineData->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_ScriptEngineData->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityBaseClass = mono_class_from_name(s_ScriptEngineData->CoreAssemblyImage, "Atom", "Entity");

		for(int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_ScriptEngineData->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_ScriptEngineData->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if(strlen(nameSpace) != 0)
			{
				fullName = fmt::format("{}.{}", nameSpace, className);
			}
			else
			{
				fullName = className;
			}

			MonoClass* monoClass = mono_class_from_name(s_ScriptEngineData->AppAssemblyImage, nameSpace, className);
			if(monoClass == nullptr)
			{
				continue;
			}

			if(monoClass == entityBaseClass)
			{
				continue;
			}

			bool isEntity = mono_class_is_subclass_of(monoClass, entityBaseClass, false);
			if(!isEntity)
			{
				continue;
			}

			ScriptClass* scriptClass = new ScriptClass(nameSpace, className);

			s_ScriptEngineData->EntityClasses[fullName] = scriptClass;

			uint32_t fieldCount = mono_class_num_fields(monoClass);

			AT_CORE_WARN("{} fields in {}", fieldCount, fullName);
			void* iterator = nullptr;
			while(MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				uint32_t flags = mono_field_get_flags(field);
				const char* fieldName = mono_field_get_name(field);

				if(!(flags & FIELD_ATTRIBUTE_PUBLIC))
				{
					continue;
				}

				MonoType* type = mono_field_get_type(field);
				ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);

				const char* fieldTypeName = Utils::ScriptFieldTypeToString(fieldType);

				AT_CORE_INFO(" - {} ({})", fieldName, Utils::ScriptFieldTypeToString(fieldType));

				scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
			}
		}
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_ScriptEngineData->CoreAssemblyImage;
	}

	MonoDomain* ScriptEngine::GetAppDomain()
	{
		return s_ScriptEngineData->AppDomain;
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_ScriptEngineData->SceneContext;
	}

	ScriptClass::ScriptClass(const std::string& classNameSpace, const std::string& className, bool isCore)
		: m_ClassNamespace(classNameSpace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_ScriptEngineData->CoreAssemblyImage : s_ScriptEngineData->AppAssemblyImage, classNameSpace.c_str(), className.c_str());
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

		m_OnCollision2DEnterMethod = s_ScriptEngineData->EntityClass.GetMethod("OnCollision2DEnter_Internal", 1);
		m_OnCollision2DExitMethod = s_ScriptEngineData->EntityClass.GetMethod("OnCollision2DExit_Internal", 1);

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