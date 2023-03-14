#pragma once
#include "Atom/Asset/AssetMetadata.h"

#include <unordered_map>

namespace Atom
{

	class AssetRegistry
	{
	public:
		AssetMetadata& operator[](const AssetHandle handle);
		const AssetMetadata& Get(const AssetHandle handle) const;

		size_t Count() const { return m_AssetMetadataMap.size(); }
		bool Contains(const AssetHandle handle) const;
		size_t Remove(const AssetHandle handle);
		void Clear();

		std::unordered_map<AssetHandle, AssetMetadata>::iterator begin() { return m_AssetMetadataMap.begin(); }
		std::unordered_map<AssetHandle, AssetMetadata>::iterator end() { return m_AssetMetadataMap.end(); }
		std::unordered_map<AssetHandle, AssetMetadata>::const_iterator begin() const { return m_AssetMetadataMap.cbegin(); }
		std::unordered_map<AssetHandle, AssetMetadata>::const_iterator end() const { return m_AssetMetadataMap.cend(); }
	private:
		std::unordered_map<AssetHandle, AssetMetadata> m_AssetMetadataMap;
	};

}
