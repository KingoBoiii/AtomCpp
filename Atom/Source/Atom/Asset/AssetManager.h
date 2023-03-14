#pragma once
#include "Registry/AssetRegistry.h"
#include "Registry/AssetRegistrySerializer.h"

namespace Atom
{

	class AssetManager
	{
	public:
		static void Initialize();
		static void Shutdown();

		static inline const AssetRegistry& GetAssetRegistry() { return s_AssetRegistry; }
	private:
		static void LoadAssetRegistry();
		static void SaveAssetRegistry();
	private:
		static inline AssetRegistry s_AssetRegistry;
	};

}
