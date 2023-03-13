#include "ATPCH.h"
#include "AssetRegistry.h"

namespace Atom
{

	AssetMetadata& AssetRegistry::operator[](const AssetHandle handle)
	{
		return m_AssetMetadataMap[handle];
	}

	const AssetMetadata& AssetRegistry::Get(const AssetHandle handle) const
	{
		AT_CORE_ASSERT(m_AssetMetadataMap.find(handle) != m_AssetMetadataMap.end());
		return m_AssetMetadataMap.at(handle);
	}

	bool AssetRegistry::Contains(const AssetHandle handle) const
	{
		return m_AssetMetadataMap.find(handle) != m_AssetMetadataMap.end();
	}

	size_t AssetRegistry::Remove(const AssetHandle handle)
	{
		return m_AssetMetadataMap.erase(handle);
	}

	void AssetRegistry::Clear()
	{
		m_AssetMetadataMap.clear();
	}

}