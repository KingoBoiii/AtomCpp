#pragma once

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

namespace Atom
{

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

	class Scene;
	class Entity;
	class ScriptClass;
	class ScriptInstance;

	class ATOM_API ScriptEngine
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void OnCreateEntity(Entity entity);
		static void OnDestroyEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float deltaTime);

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static bool EntityClassExists(const std::string& fullName);

		static std::unordered_map<std::string, ScriptClass*> GetEntityClasses();
		static ScriptInstance* GetEntityScriptInstance(UUID entityId);

		static MonoImage* GetCoreAssemblyImage();
		static MonoDomain* GetAppDomain();

		static Scene* GetSceneContext();
	private:
		static void InitializeMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField; 
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

		ScriptClass* GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
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

		inline static char s_FieldValueBuffer[8];
	};

}
