#pragma once
#include "Atom/Core/Base.h"

namespace Atom
{

	enum class AssetFlag : uint16_t
	{
		None = 0,
		Missing = BIT(0),
		Invalid = BIT(1)
	};

	inline constexpr AssetFlag operator&(AssetFlag x, AssetFlag y)
	{
		return static_cast<AssetFlag>(static_cast<uint32_t>(x) & static_cast<uint32_t>(y));
	}

	inline constexpr AssetFlag operator|(AssetFlag x, AssetFlag y)
	{
		return static_cast<AssetFlag>(static_cast<uint32_t>(x) | static_cast<uint32_t>(y));
	}

}
