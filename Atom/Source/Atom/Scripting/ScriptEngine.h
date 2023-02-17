#pragma once

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
}


namespace Atom
{

	class ATOM_API ScriptEngine
	{
	public:
		ScriptEngine();
		~ScriptEngine();

		void Initialize();
		void Shutdown();

		void LoadAssembly(const std::filesystem::path& filepath);

		inline static ScriptEngine& Get() { return *s_Instance; }
	private:
		void InitializeMono();
		void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		friend class ScriptClass;
	private:
		static ScriptEngine* s_Instance;
	};

}
