#pragma once

#include "Atom/Core/Buffer.h"

namespace Atom
{

	// TODO: Platforms
	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}
