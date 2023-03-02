#pragma once
#include "Atom/Scripting/Managed/ManagedFieldTypes.h"

#include "Atom/Core/Buffer.h"

#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoClassField MonoClassField;
}

namespace Atom
{

	enum class FieldAccessModifierFlag
	{
		None = -1,
		Public = BIT(0),
		Private = BIT(1),
		Protected = BIT(2),
		Internal = BIT(3)
	};

	enum class FieldFlag
	{
		None = BIT(0),
		ReadOnly = BIT(1),
		Static = BIT(2)

#if 1
		,
		VisibleInEditor = BIT(3),
		HiddenFromEditor = BIT(4)
#endif
	};

	namespace Utils
	{

		static std::string GetFieldAccessModifierName(FieldAccessModifierFlag accessModifier)
		{
			switch(accessModifier)
			{
				case FieldAccessModifierFlag::Public: return "public";
				case FieldAccessModifierFlag::Private: return "private";
				case FieldAccessModifierFlag::Protected: return "protected";
				case FieldAccessModifierFlag::Internal: return "internal";
				default: return "unknown";
			}
		}

		static std::string GetFieldFlagName(FieldFlag fieldFlag)
		{
			switch(fieldFlag)
			{
				case FieldFlag::Static: return "Static";
				case FieldFlag::ReadOnly: return "ReadOnly";
				case FieldFlag::None:
				default: return "None";
			}
		}

	}

	class ManagedClassField
	{
	public:
		ManagedClassField() = default;
		ManagedClassField(const ManagedClassField&) = default;
		ManagedClassField(MonoClassField* rawClassField);
		~ManagedClassField();

		void SetAccessModifierByAttribute(void* attributes, MonoClass* attributeClass, FieldAccessModifierFlag accessModifierFlag);

		bool HasFieldFlag(FieldFlag fieldFlag) const { return (uint64_t)m_Flags & (uint64_t)fieldFlag; }
		bool HasFieldAccessModifier(FieldAccessModifierFlag fieldAccessModifierFlag) const { return (uint64_t)m_AccessModifier & (uint64_t)fieldAccessModifierFlag; }

		MonoClassField* GetRawClassField() const { return m_RawClassField; }
		std::string GetFullName() const { return m_Name; }
		ManagedFieldType GetType() const { return m_Type; }
		std::string GetTypeString() const { return Utils::ManagedFieldTypeToString(m_Type); }

		FieldAccessModifierFlag GetFieldAccessModifier() const { return m_AccessModifier; }
		std::string GetFieldAccessModifierString() const { return Utils::GetFieldAccessModifierName(m_AccessModifier); }

		FieldFlag GetFieldFlags() const { return m_Flags; }
		std::string GetFieldFlagsString() const { return Utils::GetFieldFlagName(m_Flags); }

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "ManagedClassField::SetValue: Value size is too big!");
			memcpy(m_FieldValueBuffer.Data, &value, sizeof(T));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "ManagedClassField::GetValue: Value size is too big!");
			return *m_FieldValueBuffer.As<T>();
		}
	private:
		void LoadField();
	private:
		std::string m_Name = "";
		ManagedFieldType m_Type = ManagedFieldType::None;
		FieldAccessModifierFlag m_AccessModifier = FieldAccessModifierFlag::None;
		FieldFlag m_Flags = FieldFlag::None;
		Buffer m_FieldValueBuffer;

		MonoClassField* m_RawClassField = nullptr;

		friend class ScriptEngine;
	};

#if 0
	struct ManagedClassField
	{
		ManagedFieldType Type;
		std::string Name;
		FieldAccessModifierFlag AccessModifier;
		FieldFlag Flags;
		Buffer DefaultValueBuffer;
		//MonoClassField* ClassField;
	};
#endif

}
