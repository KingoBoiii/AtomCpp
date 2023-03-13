#pragma once
#include "Atom/Core/UUID.h"

#include "AssetFlag.h"
#include "AssetType.h"

namespace Atom
{

	using AssetHandle = UUID;

	struct Asset
	{
		virtual ~Asset() = default;

		AssetHandle Handle = 0;
		AssetFlag Flags = AssetFlag::None;

		virtual bool operator==(const Asset& other) const
		{
			return Handle == other.Handle;
		}

		virtual bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}
	};

}
