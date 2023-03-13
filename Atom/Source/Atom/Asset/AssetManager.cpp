#include "ATPCH.h"
#include "AssetManager.h"
#include "AssetExtensions.h"

#include "AssetMetadata.h"

#include "Atom/Project/Project.h"

namespace Atom
{

	void AssetManager::Initialize()
	{
		const auto& assetDirectoryPath = Project::GetAssetDirectory();
		
		for(auto& directoryEntry : std::filesystem::recursive_directory_iterator(assetDirectoryPath))
		{
			if(directoryEntry.is_directory())
			{
				continue;
			}

			const std::filesystem::path& currentPath = directoryEntry.path();

			AssetMetadata assetMetadata{ };
			assetMetadata.Handle = AssetHandle();
			assetMetadata.Type = s_AssetExtensionMap.at(currentPath.extension().string());
			assetMetadata.Filepath = currentPath;

			s_AssetRegistry[assetMetadata.Handle] = assetMetadata;
#if 0
			AT_CORE_TRACE("Asset {}", currentPath.filename().stem().string());
			AT_CORE_TRACE("- Handle: {}", assetMetadata.Handle);
			AT_CORE_TRACE("- Type: {}", Utils::AssetTypeToString(assetMetadata.Type));
			AT_CORE_TRACE("- Filepath: {}", assetMetadata.Filepath.string());
#endif
		}

		AT_CORE_INFO("[AssetManager] Loaded {0} asset entries", s_AssetRegistry.Count());
	}

	void AssetManager::Shutdown()
	{
	}

}