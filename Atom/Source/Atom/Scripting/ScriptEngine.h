#pragma once

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoDomain MonoDomain;
}

namespace Atom
{

	class Scene;
	class Entity;
	class ScriptClass;

	class ATOM_API ScriptEngine
	{
	public:
		ScriptEngine(Scene* m_Scene);
		~ScriptEngine();

		void Initialize();
		void Shutdown();

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnCreateEntity(Entity entity);
		void OnDestroyEntity(Entity entity);
		void OnUpdateEntity(Entity entity, float deltaTime);

		void LoadAssembly(const std::filesystem::path& filepath);

		bool EntityClassExists(const std::string& fullName);

		static std::unordered_map<std::string, ScriptClass*> GetEntityClasses();

		static MonoImage* GetCoreAssemblyImage();
		static MonoDomain* GetAppDomain();

		inline static Scene* GetScene() { return s_Instance->m_Scene; }
		inline static ScriptEngine& Get() { return *s_Instance; }
	private:
		void InitializeMono();
		void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses(MonoAssembly* assembly, MonoImage* image);

		friend class ScriptClass;
		friend class ScriptGlue;
	private:
		Scene* m_Scene;
	private:
		static ScriptEngine* s_Instance;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNameSpace, const std::string& className);

		MonoObject* Instantiate() const;
		MonoMethod* GetMethod(const std::string& methodName, int parameterCount = 0) const;
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* monoMethod, void** parameters = nullptr);
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(ScriptClass* scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnDestroy();
		void InvokeOnUpdate(float deltaTime);
	private:
		ScriptClass* m_ScriptClass = nullptr;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnDestroyMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
	};
	
}
