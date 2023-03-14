#pragma once
#include "Registry/AssetRegistry.h"
#include "Registry/AssetRegistrySerializer.h"

namespace Atom
{

	enum class AssetEvent
	{
		Added,
		Removed,
		Modified,
		RenamedOld,
		RenamedNew
	};

	class AssetManager
	{
	public:
		static void Initialize();
		static void Shutdown();

		static AssetHandle GetAssetHandle(const std::filesystem::path& filepath);
		static const AssetMetadata& GetAssetMetadata(const std::filesystem::path& filepath);

		static std::filesystem::path GetRelativePath(const std::filesystem::path& filepath);

		static const AssetRegistry& GetAssetRegistry() { return s_AssetRegistry; }
	private:
		static void OnAssetDirectoryChanged(const std::filesystem::path& filepath, AssetEvent changeType);
		static void OnAssetAdded(const std::filesystem::path& filepath);
		static void OnAssetRemoved(const std::filesystem::path& filepath);
	private:
		static void LoadAssetRegistry();
		static void SaveAssetRegistry();
	private:
		static inline AssetRegistry s_AssetRegistry;
	};

}
