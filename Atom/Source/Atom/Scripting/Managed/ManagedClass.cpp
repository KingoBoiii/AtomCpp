#include "ATPCH.h"
#include "ManagedClass.h"

#include "Atom/Scripting/ScriptCache.h"
#include "Atom/Scripting/ScriptUtils.h"

#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace Atom
{

	namespace Utils
	{

		void SetDefaultValue(MonoObject* instance, MonoClassField* monoClassField, ManagedClassField& managedClassField)
		{
			uint8_t* buffer = new uint8_t[16];
			mono_field_get_value(instance, monoClassField, buffer);

			MonoClass* monoClass = mono_object_get_class(instance);
			std::string classNamespace = mono_class_get_namespace(monoClass);
			std::string className = mono_class_get_name(monoClass);
			std::string fullName = fmt::format("{}.{}:{}", classNamespace, className, managedClassField.GetFullName());

#define SET_DEFAULT_VALUE_CASE(managedType, type) case managedType: managedClassField.SetValue<type>(*(type*)buffer); AT_CORE_WARN("{}: {}", fullName, *(type*)buffer); return

			switch(managedClassField.GetType())
			{
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Bool, bool);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Char, char);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Float, float);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Double, double);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Byte, uint8_t);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Short, int16_t);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Int, int32_t);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::Long, int64_t);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::SByte, int8_t);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::UShort, uint16_t);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::UInt, uint32_t);
				SET_DEFAULT_VALUE_CASE(Atom::ManagedFieldType::ULong, uint64_t);
				case Atom::ManagedFieldType::Vector2:
				case Atom::ManagedFieldType::Vector3:
				case Atom::ManagedFieldType::Vector4:
				case Atom::ManagedFieldType::Entity:
				case Atom::ManagedFieldType::String:
				case Atom::ManagedFieldType::None:
				default: AT_CORE_WARN("Unsupport Managed Field type!"); break;
			}
		}

	}

	ManagedClass::ManagedClass(MonoClass* rawClass)
		: m_RawClass(rawClass)
	{
		m_ClassNamespace = mono_class_get_namespace(rawClass);
		m_ClassName = mono_class_get_name(rawClass);

		LoadFields();
	}

	ManagedClass::~ManagedClass()
	{
		m_RawClass = nullptr;
	}

	ManagedClassField ManagedClass::GetField(const std::string& fieldName) const
	{
		auto iterator = m_Fields.find(fieldName);
		if(iterator == m_Fields.end())
		{
			return ManagedClassField();
		}

		return iterator->second;
	}

	void ManagedClass::LoadFields()
	{
		if(m_ClassNamespace == "System")
		{
			return;
		}

		int32_t fieldCount = mono_class_num_fields(m_RawClass);

		AT_CORE_INFO("{} fields in {}", fieldCount, GetFullName());

		MonoObject* temporaryInstance = ScriptUtils::InstantiateManagedClass(this);

		void* iterator = nullptr;
		while(MonoClassField* monoClassField = mono_class_get_fields(m_RawClass, &iterator))
		{
			ManagedClassField managedClassField(monoClassField);

			MonoCustomAttrInfo* attributes = mono_custom_attrs_from_field(m_RawClass, monoClassField);

			managedClassField.SetAccessModifierByAttribute(attributes, AT_CACHED_CLASS_RAW("Atom.VisibleInEditorAttribute"), FieldAccessModifierFlag::Public);
			managedClassField.SetAccessModifierByAttribute(attributes, AT_CACHED_CLASS_RAW("Atom.HiddenFromEditorAttribute"), FieldAccessModifierFlag::Private);

			Utils::SetDefaultValue(temporaryInstance, monoClassField, managedClassField);

			m_Fields[managedClassField.GetFullName()] = managedClassField;
		}

		//mono_free(temporaryInstance);
		temporaryInstance = nullptr;
	}

}