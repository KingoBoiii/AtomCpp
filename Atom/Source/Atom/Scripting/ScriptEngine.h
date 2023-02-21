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
		T GetValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 8, "Type too large");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 8, "Type too large");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[8];

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
		static void Initialize();
		static void Shutdown();

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

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 8, "Type too large");
			
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
			static_assert(sizeof(T) <= 8, "Type too large");

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

		inline static char s_FieldValueBuffer[8];

		friend struct ScriptFieldInstance;
		friend class ScriptEngine;
	};

}
