#pragma once
#include "Assembly/AssemblyInfo.h"
#include "Assembly/AssemblyMetadata.h"

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
	};

	enum class ScriptFieldType
	{
		None = 0,
		Bool,
		Char, String,
		Float, Double,
		Byte, Short, Int, Long,
		Vector2, Vector3, Vector4,
		Entity
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};

	// ScriptField + Data storage
	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];

		friend class ScriptInstance;
		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class Scene;
	class Entity;
	class ScriptClass;
	class ScriptInstance;

	class ATOM_API ScriptEngine
	{
	public:
		static void Initialize(const ScriptEngineConfig& config);
		static void Shutdown();
		static void ReloadAssembly();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void OnCreateEntity(Entity entity);
		static void OnDestroyEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float deltaTime);

		static void InvokeOnCollection2DEnter(Entity entity, Entity other);
		static void InvokeOnCollection2DExit(Entity entity, Entity other);

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static bool EntityClassExists(const std::string& fullName);

		static ScriptClass* GetEntityClass(const std::string& name);
		static std::unordered_map<std::string, ScriptClass*> GetEntityClasses();
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);

		static ScriptInstance* GetEntityScriptInstance(UUID entityId);

		static MonoObject* GetManagedInstance(UUID entityId);

		//static MonoImage* GetCoreAssemblyImage();
		static MonoDomain* GetAppDomain();

		static Scene* GetSceneContext();

		static AssemblyInfo* GetCoreAssemblyInfo();
		static AssemblyInfo* GetAppAssemblyInfo();
	private:
		static void InitializeMono();
		static void ShutdownMono();

		static bool LoadCoreAssembly();
		static bool LoadAppAssembly();
		static AssemblyMetadata LoadAssemblyMetadata(MonoImage* image);
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& filepath);

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNameSpace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate() const;
		MonoMethod* GetMethod(const std::string& methodName, int parameterCount = 0) const;
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* monoMethod, void** parameters = nullptr);

		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::map<std::string, ScriptField> m_Fields;

		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(ScriptClass* scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnDestroy();
		void InvokeOnUpdate(float deltaTime);

		void InvokeOnCollision2DEnter(Entity other);
		void InvokeOnCollision2DExit(Entity other);

		ScriptClass* GetScriptClass() { return m_ScriptClass; }
		MonoObject* GetManagedObject() { return m_Instance; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large");
			
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if(!success)
			{
				return T();
			}

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			static_assert(sizeof(T) <= 16, "Type too large");

			SetFieldValueInternal(name, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		ScriptClass* m_ScriptClass = nullptr;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnDestroyMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		MonoMethod* m_OnCollision2DEnterMethod = nullptr;
		MonoMethod* m_OnCollision2DExitMethod = nullptr;

		inline static char s_FieldValueBuffer[16];

		friend struct ScriptFieldInstance;
		friend class ScriptEngine;
	};

	namespace Utils
	{

		inline const char* ScriptFieldTypeToString(ScriptFieldType scriptFieldType)
		{
			switch(scriptFieldType)
			{
				case Atom::ScriptFieldType::Bool:		return "Bool";
				case Atom::ScriptFieldType::Char:		return "Char";
				case Atom::ScriptFieldType::String:		return "String";
				case Atom::ScriptFieldType::Float:		return "Float";
				case Atom::ScriptFieldType::Double:		return "Double";
				case Atom::ScriptFieldType::Byte:		return "Byte";
				case Atom::ScriptFieldType::Short:		return "Short";
				case Atom::ScriptFieldType::Int:		return "Int";
				case Atom::ScriptFieldType::Long:		return "Long";
				case Atom::ScriptFieldType::Vector2:	return "Vector2";
				case Atom::ScriptFieldType::Vector3:	return "Vector3";
				case Atom::ScriptFieldType::Vector4:	return "Vector4";
				case Atom::ScriptFieldType::Entity:		return "Entity";
				case Atom::ScriptFieldType::None:		return "None";
				default: break;
			}
			
			AT_CORE_ASSERT(false, "Unknown Script Field Type!");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view scriptFieldType)
		{
			if(scriptFieldType == "None")		return ScriptFieldType::None;
			if(scriptFieldType == "Bool")		return ScriptFieldType::Bool;
			if(scriptFieldType == "Char")		return ScriptFieldType::Char;
			if(scriptFieldType == "String")		return ScriptFieldType::String;
			if(scriptFieldType == "Float")		return ScriptFieldType::Float;
			if(scriptFieldType == "Double")		return ScriptFieldType::Double;
			if(scriptFieldType == "Byte")		return ScriptFieldType::Byte;
			if(scriptFieldType == "Short")		return ScriptFieldType::Short;
			if(scriptFieldType == "Int")		return ScriptFieldType::Int;
			if(scriptFieldType == "Long")		return ScriptFieldType::Long;
			if(scriptFieldType == "Vector2")	return ScriptFieldType::Vector2;
			if(scriptFieldType == "Vector3")	return ScriptFieldType::Vector3;
			if(scriptFieldType == "Vector4")	return ScriptFieldType::Vector4;
			if(scriptFieldType == "Entity")		return ScriptFieldType::Entity;

			return ScriptFieldType::None;
		}

	}

}
