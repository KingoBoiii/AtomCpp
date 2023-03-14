#pragma once
#include "AssetType.h"

#include <unordered_map>

#define ATOM_SCENE_FILE_EXTENSION	".atsc"
#define ATOM_SCRIPT_FILE_EXTENSION	".cs"

#define ATOM_JPG_TEXTURE_EXTENSION	".jpg"

namespace Atom
{

	static inline std::unordered_map<std::string, AssetType> s_AssetExtensionMap = 
	{
		{ ATOM_SCENE_FILE_EXTENSION,	AssetType::Scene },
		
		{ ATOM_JPG_TEXTURE_EXTENSION,	AssetType::Texture },

		{ ATOM_SCRIPT_FILE_EXTENSION,	AssetType::Script }
	};

}
