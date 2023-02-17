#pragma once

extern "C" {
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
}

namespace Atom
{

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(MonoImage* monoImage, const std::string& classNameSpace, const std::string& className);

		MonoObject* Instantiate() const;
		MonoMethod* GetMethod(const std::string& methodName, int parameterCount = 0) const;
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* monoMethod, void** parameters = nullptr);
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		MonoImage* m_MonoImage = nullptr;
		MonoClass* m_MonoClass = nullptr;
	};

}
