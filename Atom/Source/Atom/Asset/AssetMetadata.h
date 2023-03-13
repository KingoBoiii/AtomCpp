#pragma once
#include "Asset.h"

namespace Atom
{
	
	struct AssetMetadata
	{
		AssetHandle Handle = 0;
		AssetType Type;
		std::filesystem::path Filepath;
	};

}
