#pragma once
#include "Atom/Core/Base.h"

namespace Atom
{
	
	enum class BindFlags : uint32_t
	{
		None = 0,
		VertexBuffer	= BIT(1),
		IndexBuffer		= BIT(2),
		ConstantBuffer	= BIT(3),
		ShaderResource	= BIT(4),
		RenderTarget	= BIT(5),
		DepthStencil	= BIT(6)
	};

	inline constexpr BindFlags operator&(BindFlags x, BindFlags y)
	{
		return static_cast<BindFlags>(static_cast<uint32_t>(x) & static_cast<uint32_t>(y));
	}

	inline constexpr BindFlags operator|(BindFlags x, BindFlags y)
	{
		return static_cast<BindFlags>(static_cast<uint32_t>(x) | static_cast<uint32_t>(y));
	}
	
}