#pragma once

#include "AssemblyInfo.h"
#include "AssemblyMetadata.h"

namespace Atom
{

	class AssemblyUtils
	{
	public:
		static AssemblyInfo* LoadAssembly(const std::filesystem::path& filepath, bool enableDebugging = false);

		static AssemblyMetadata LoadAssemblyMetadata(AssemblyInfo* assemblyInfo);
		static std::vector<AssemblyMetadata> GetReferencedAssembliesMetadata(AssemblyInfo* assemblyInfo);
	};

}
