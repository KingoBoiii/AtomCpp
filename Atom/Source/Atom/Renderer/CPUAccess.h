#pragma once

namespace Atom
{

	enum class CPUAccess
	{
		None = 0,
		Read,
		Write,
		
		ReadAndWrite = Read | Write
	};

}