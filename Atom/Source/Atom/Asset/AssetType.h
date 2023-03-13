#pragma once
#include "Atom/Core/Base.h"

namespace Atom
{

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Script
	};

	namespace Utils
	{

		static inline AssetType AssetTypeFromString(const std::string& assetType)
		{
			if(assetType == "None")				return AssetType::None;
			if(assetType == "Scene")			return AssetType::Scene;
			if(assetType == "Script")			return AssetType::Scene;

			AT_CORE_ASSERT(false, "Unknown Asset Type");
			return AssetType::None;
		}

		static inline std::string AssetTypeToString(AssetType assetType)
		{
			switch(assetType)
			{
				case Atom::AssetType::None:		return "None";
				case Atom::AssetType::Scene:	return "Scene";
				case Atom::AssetType::Script:	return "Script";
			}

			AT_CORE_ASSERT(false, "Unknown Asset Type");
			return "None";
		}

	}

}
