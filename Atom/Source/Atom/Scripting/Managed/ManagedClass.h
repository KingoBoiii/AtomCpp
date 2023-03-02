#pragma once
#include "Atom/Scripting/Assembly/AssemblyInfo.h"
#include "ManagedClassField.h"

#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
}

#undef GetClassName

namespace Atom
{

	class ManagedClass
	{
	public:
		ManagedClass() = default;
		ManagedClass(const ManagedClass&) = default;
		ManagedClass(MonoClass* rawClass);
		~ManagedClass();

		MonoClass* GetRawClass() const { return m_RawClass; }
		const std::string& GetClassName() const { return m_ClassName; }
		const std::string& GetClassNamespace() const { return m_ClassNamespace; }
		std::string GetFullName() const { return fmt::format("{}.{}", m_ClassNamespace, m_ClassName); }

		ManagedClassField GetField(const std::string& fieldName) const;
		const std::unordered_map<std::string, ManagedClassField>& GetFields() { return m_Fields; }
		const std::unordered_map<std::string, ManagedClassField>& GetFields() const { return m_Fields; }
	private:
		void LoadFields();
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::unordered_map<std::string, ManagedClassField> m_Fields;

		MonoClass* m_RawClass = nullptr;
	};

}
