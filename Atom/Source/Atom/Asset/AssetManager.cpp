#include "ATPCH.h"
#include "AssetManager.h"
#include "AssetExtensions.h"
#include "Registry/AssetRegistrySerializer.h"

#include "AssetMetadata.h"

#include "Atom/Project/Project.h"

#include "FileWatch.h"

namespace Atom
{

	namespace Utils
	{

		static inline filewatch::Event FilewatchEventFromAssetEvent(AssetEvent assetEvent)
		{
			switch(assetEvent)
			{
				case AssetEvent::Added:			return filewatch::Event::added;
				case AssetEvent::Removed:		return filewatch::Event::removed;
				case AssetEvent::Modified:		return filewatch::Event::modified;
				case AssetEvent::RenamedOld:	return filewatch::Event::renamed_old;
				case AssetEvent::RenamedNew:	return filewatch::Event::renamed_new;
				default: break;
			}

			AT_CORE_ASSERT(false, "Unknown Asset Event!");
		}

		static inline AssetEvent AssetEventFromFilewatchEvent(filewatch::Event filewatchEvent)
		{
			switch(filewatchEvent)
			{
				case filewatch::Event::added:		return AssetEvent::Added;
				case filewatch::Event::removed:		return AssetEvent::Removed;
				case filewatch::Event::modified:	return AssetEvent::Modified;
				case filewatch::Event::renamed_old:	return AssetEvent::RenamedOld;
				case filewatch::Event::renamed_new:	return AssetEvent::RenamedNew;
				default: break;
			}

			AT_CORE_ASSERT(false, "Unknown Asset Event!");
		}

	}

#define LOAD_ASSETS_FROM_FILESYSTEM 0

	struct AssetManagerData
	{
		filewatch::FileWatch<std::string>* AssetDirectroyWatcher = nullptr;
	};

	static AssetManagerData* s_AssetManagerData = nullptr;
	static AssetMetadata s_NullMetadata;

	void AssetManager::Initialize()
	{
		s_AssetManagerData = new AssetManagerData();
		const auto& assetDirectoryPath = Project::GetAssetDirectory();

#if LOAD_ASSETS_FROM_FILESYSTEM
		for(auto& directoryEntry : std::filesystem::recursive_directory_iterator(assetDirectoryPath))
		{
			if(directoryEntry.is_directory())
			{
				continue;
			}

			const std::filesystem::path& currentPath = directoryEntry.path();

			std::filesystem::path relativePath = std::filesystem::relative(currentPath, Project::GetAssetDirectory());

			AssetMetadata assetMetadata{ };
			assetMetadata.Handle = AssetHandle();
			assetMetadata.Type = s_AssetExtensionMap.at(relativePath.extension().string());
			assetMetadata.Filepath = relativePath;

			s_AssetManagerData->AssetRegistry[assetMetadata.Handle] = assetMetadata;
		}
#else 
		LoadAssetRegistry();
#endif
		s_AssetManagerData->AssetDirectroyWatcher = new filewatch::FileWatch<std::string>(assetDirectoryPath.string(), [](const std::string& path, filewatch::Event changeType)
		{
			OnAssetDirectoryChanged(path, Utils::AssetEventFromFilewatchEvent(changeType));
		});
		
		AT_CORE_INFO("[AssetManager] Loaded {0} asset entries", s_AssetRegistry.Count());
	}

	void AssetManager::Shutdown()
	{
		SaveAssetRegistry();

		delete s_AssetManagerData->AssetDirectroyWatcher;
		delete s_AssetManagerData;
	}

	AssetHandle AssetManager::GetAssetHandle(const std::filesystem::path& filepath)
	{
		return GetAssetMetadata(filepath).Handle;
	}

	const AssetMetadata& AssetManager::GetAssetMetadata(const std::filesystem::path& filepath)
	{
		const auto& relativePath = GetRelativePath(filepath);

		for(auto& [handle, metadata] : s_AssetRegistry)
		{
			if(metadata.Filepath == relativePath)
			{
				return metadata;
			}
		}

		return s_NullMetadata;
	}

	std::filesystem::path AssetManager::GetRelativePath(const std::filesystem::path& filepath)
	{
		std::filesystem::path relativePath = filepath.lexically_normal();
		std::string temp = filepath.string();
		if(temp.find(Project::GetAssetDirectory().string()) != std::string::npos)
		{
			relativePath = std::filesystem::relative(filepath, Project::GetAssetDirectory());
			if(relativePath.empty())
			{
				relativePath = filepath.lexically_normal();
			}
		}
		return relativePath;
	}

	void AssetManager::OnAssetDirectoryChanged(const std::filesystem::path& filepath, AssetEvent changeType)
	{
		AT_CORE_TRACE("[AssetManager] Asset Directory Changed: ({}) {}", (int32_t)changeType, filepath);

		switch(changeType)
		{
			case AssetEvent::Added:			OnAssetAdded(filepath);		break;
			case AssetEvent::Removed:		OnAssetRemoved(filepath);	break;
			case AssetEvent::Modified:		break;
			case AssetEvent::RenamedOld:	break;
			case AssetEvent::RenamedNew:	break;
			default: break;
		}
	}

	void AssetManager::OnAssetAdded(const std::filesystem::path& filepath)
	{
		AssetType type = AssetType::None;
		if(s_AssetExtensionMap.find(std::filesystem::path(filepath).extension().string()) != s_AssetExtensionMap.end())
		{
			type = s_AssetExtensionMap.at(std::filesystem::path(filepath).extension().string());
		}

		AssetMetadata assetMetadata{};
		assetMetadata.Handle = AssetHandle();
		assetMetadata.Type = type;
		assetMetadata.Filepath = filepath;

		s_AssetRegistry[assetMetadata.Handle] = assetMetadata;
	}

	void AssetManager::OnAssetRemoved(const std::filesystem::path& filepath)
	{
		AssetHandle handle = AssetManager::GetAssetHandle(filepath);
		s_AssetRegistry.Remove(handle);
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