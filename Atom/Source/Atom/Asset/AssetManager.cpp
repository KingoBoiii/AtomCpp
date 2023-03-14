#include "ATPCH.h"
#include "AssetManager.h"
#include "AssetExtensions.h"
#include "Registry/AssetRegistrySerializer.h"

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
		}

		LoadAssetRegistry();

		AT_CORE_INFO("[AssetManager] Loaded {0} asset entries", s_AssetRegistry.Count());
	}

	void AssetManager::Shutdown()
	{
		SaveAssetRegistry();
	}

	void AssetManager::LoadAssetRegistry()
	{
		const auto& assetRegistryPath = Project::GetAssetRegistryPath();
		if(!std::filesystem::exists(assetRegistryPath))
		{
			AT_CORE_WARN("[AssetManager] Asset Registry file does not exist...");
			return;
		}

		AssetRegistrySerializer serializer = AssetRegistrySerializer(&s_AssetRegistry);
		serializer.Deserialize(assetRegistryPath);
	}

	void AssetManager::SaveAssetRegistry()
	{
		if(&s_AssetRegistry == nullptr)
		{
			return;
		}

		const auto& assetRegistryPath = Project::GetAssetRegistryPath();

		AssetRegistrySerializer serializer = AssetRegistrySerializer(&s_AssetRegistry);
		serializer.Serialize(assetRegistryPath);
	}

}