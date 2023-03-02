#include "ATPCH.h"
#include "ManagedClassField.h"

#include <mono/metadata/class.h>
#include <mono/metadata/tabledefs.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/reflection.h>

namespace Atom
{
	
	namespace Utils
	{

		static FieldAccessModifierFlag GetFieldAccessModifier(MonoClassField* field)
		{
			uint32_t fieldAccessFlags = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

			if(fieldAccessFlags & FIELD_ATTRIBUTE_PUBLIC)
			{
				return FieldAccessModifierFlag::Public;
			}
			else if(fieldAccessFlags & FIELD_ATTRIBUTE_PRIVATE)
			{
				return FieldAccessModifierFlag::Private;
			}
			else if(fieldAccessFlags & FIELD_ATTRIBUTE_FAMILY)
			{
				return FieldAccessModifierFlag::Protected;
			}
			else if(fieldAccessFlags & FIELD_ATTRIBUTE_ASSEMBLY)
			{
				return FieldAccessModifierFlag::Internal;
			}

			return FieldAccessModifierFlag::Public;
		}

		static FieldFlag GetFieldFlags(MonoClassField* field)
		{
			uint64_t flags = (uint64_t)FieldFlag::None;

			uint32_t fieldFlags = mono_field_get_flags(field);

			switch(fieldFlags)
			{
				case MONO_FIELD_ATTR_STATIC:
				{
					flags &= ~(uint64_t)FieldFlag::ReadOnly;
					flags |= (uint64_t)FieldFlag::Static;
					break;
				}
			}

			return (FieldFlag)flags;
		}

	}

	ManagedClassField::ManagedClassField(MonoClassField* rawClassField)
		: m_RawClassField(rawClassField)
	{
		LoadField();
	}

	ManagedClassField::~ManagedClassField()
	{
		m_RawClassField = nullptr;
	}

	void ManagedClassField::SetAccessModifierByAttribute(void* attributes, MonoClass* attributeClass, FieldAccessModifierFlag accessModifierFlag)
	{
		if((MonoCustomAttrInfo*)attributes && mono_custom_attrs_has_attr((MonoCustomAttrInfo*)attributes, attributeClass))
		{
			m_AccessModifier = accessModifierFlag;
		}
	}

	void ManagedClassField::LoadField()
	{
		std::string fieldName = mono_field_get_name(m_RawClassField);

		// Properties have a backing field called <PropertyName>k__BackingField. We don't want to include those in the class fields list.
		if(fieldName.find("k__BackingField") != std::string::npos)
		{
			return;
		}

		MonoType* monoType = mono_field_get_type(m_RawClassField);

		ManagedFieldType fieldType = Utils::MonoTypeToScriptFieldType(monoType);

		std::string fieldTypeName = Utils::ManagedFieldTypeToString(fieldType);

		FieldAccessModifierFlag accessModifier = Utils::GetFieldAccessModifier(m_RawClassField);
		FieldFlag fieldFlags = Utils::GetFieldFlags(m_RawClassField);

		m_Name = fieldName;
		m_Type = fieldType;
		m_AccessModifier = accessModifier;
		m_Flags = fieldFlags;

		m_FieldValueBuffer.Allocate(16);
		memset(m_FieldValueBuffer.Data, 0, sizeof(m_FieldValueBuffer.Data));
	}

}