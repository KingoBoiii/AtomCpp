#pragma once
#include <filesystem>
#include "AssemblyMetadata.h"

extern "C" {
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}

namespace Atom
{

	struct AssemblyInfo
	{
		std::filesystem::path Filepath = "";
		MonoAssembly* Assembly = nullptr;
		MonoImage* AssemblyImage = nullptr;
		AssemblyMetadata Metadata;
		bool IsCoreAssembly = false;
	}; 

}
