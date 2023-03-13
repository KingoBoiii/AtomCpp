#pragma once
#include "AssetType.h"

#include <unordered_map>

#define ATOM_SCENE_FILE_EXTENSION	".atsc"
#define ATOM_SCRIPT_FILE_EXTENSION	".cs"

namespace Atom
{

	static inline std::unordered_map<std::string, AssetType> s_AssetExtensionMap = 
	{
		{ ATOM_SCENE_FILE_EXTENSION,	AssetType::Scene },
		
		{ ATOM_SCRIPT_FILE_EXTENSION,	AssetType::Script }
	};

}
