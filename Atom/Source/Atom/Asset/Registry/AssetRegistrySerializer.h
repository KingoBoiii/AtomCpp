#pragma once
#include "AssetRegistry.h"

namespace Atom
{
	
	class AssetRegistrySerializer
	{
	public:
		AssetRegistrySerializer(AssetRegistry* assetRegistry);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	private:
		AssetRegistry* m_AssetRegistry;
	};

}
