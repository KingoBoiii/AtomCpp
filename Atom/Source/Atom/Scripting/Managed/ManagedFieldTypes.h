#pragma once

#include <string>
#include <unordered_map>

extern "C" {
	typedef struct _MonoType MonoType;
}

namespace Atom
{

	enum class ManagedFieldType
	{
		None = 0,
		Bool,
		Char, String,
		Float, Double,
		Byte, Short, Int, Long,
		SByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	using ScriptFieldTypeMap = std::unordered_map<std::string, ManagedFieldType>;
	static ScriptFieldTypeMap s_ScriptFieldTypeMap = {
		{ "System.Boolean",		ManagedFieldType::Bool },
		{ "System.Char",		ManagedFieldType::Char },
		{ "System.String",		ManagedFieldType::String },
		{ "System.Single",		ManagedFieldType::Float },
		{ "System.Double",		ManagedFieldType::Double },
		{ "System.Byte",		ManagedFieldType::Byte },
		{ "System.Int16",		ManagedFieldType::Short },
		{ "System.Int32",		ManagedFieldType::Int },
		{ "System.Int64",		ManagedFieldType::Long },
		{ "System.SByte",		ManagedFieldType::SByte },
		{ "System.UInt16",		ManagedFieldType::UShort },
		{ "System.UInt32",		ManagedFieldType::UInt },
		{ "System.UInt64",		ManagedFieldType::ULong },
		{ "Atom.Vector2",		ManagedFieldType::Vector2 },
		{ "Atom.Vector3",		ManagedFieldType::Vector3 },
		{ "Atom.Vector4",		ManagedFieldType::Vector4 },
		{ "Atom.Entity",		ManagedFieldType::Entity }
	};

	namespace Utils
	{

		ManagedFieldType MonoTypeToScriptFieldType(MonoType* monoType);

		inline const char* ManagedFieldTypeToString(ManagedFieldType scriptFieldType)
		{
			switch(scriptFieldType)
			{
				case Atom::ManagedFieldType::Bool:		return "Bool";
				case Atom::ManagedFieldType::Char:		return "Char";
				case Atom::ManagedFieldType::String:	return "String";
				case Atom::ManagedFieldType::Float:		return "Float";
				case Atom::ManagedFieldType::Double:	return "Double";
				case Atom::ManagedFieldType::Byte:		return "Byte";
				case Atom::ManagedFieldType::Short:		return "Short";
				case Atom::ManagedFieldType::Int:		return "Int";
				case Atom::ManagedFieldType::Long:		return "Long";
				case Atom::ManagedFieldType::SByte:		return "SByte";
				case Atom::ManagedFieldType::UShort:	return "UShort";
				case Atom::ManagedFieldType::UInt:		return "UInt";
				case Atom::ManagedFieldType::ULong:		return "ULong";
				case Atom::ManagedFieldType::Vector2:	return "Vector2";
				case Atom::ManagedFieldType::Vector3:	return "Vector3";
				case Atom::ManagedFieldType::Vector4:	return "Vector4";
				case Atom::ManagedFieldType::Entity:	return "Entity";
				case Atom::ManagedFieldType::None:		return "None";
				default: break;
			}

			AT_CORE_ASSERT(false, "Unknown Script Field Type!");
			return "None";
		}

		inline ManagedFieldType ManagedFieldTypeFromString(std::string_view scriptFieldType)
		{
			if(scriptFieldType == "None")		return ManagedFieldType::None;
			if(scriptFieldType == "Bool")		return ManagedFieldType::Bool;
			if(scriptFieldType == "Char")		return ManagedFieldType::Char;
			if(scriptFieldType == "String")		return ManagedFieldType::String;
			if(scriptFieldType == "Float")		return ManagedFieldType::Float;
			if(scriptFieldType == "Double")		return ManagedFieldType::Double;
			if(scriptFieldType == "Byte")		return ManagedFieldType::Byte;
			if(scriptFieldType == "Short")		return ManagedFieldType::Short;
			if(scriptFieldType == "Int")		return ManagedFieldType::Int;
			if(scriptFieldType == "Long")		return ManagedFieldType::Long;
			if(scriptFieldType == "SByte")		return ManagedFieldType::SByte;
			if(scriptFieldType == "UShort")		return ManagedFieldType::UShort;
			if(scriptFieldType == "UInt")		return ManagedFieldType::UInt;
			if(scriptFieldType == "ULong")		return ManagedFieldType::ULong;
			if(scriptFieldType == "Vector2")	return ManagedFieldType::Vector2;
			if(scriptFieldType == "Vector3")	return ManagedFieldType::Vector3;
			if(scriptFieldType == "Vector4")	return ManagedFieldType::Vector4;
			if(scriptFieldType == "Entity")		return ManagedFieldType::Entity;

			return ManagedFieldType::None;
		}

	}

}
