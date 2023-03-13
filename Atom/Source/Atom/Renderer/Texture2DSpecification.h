#pragma once
#include "Flags/Usage.h"
#include "Flags/BindFlags.h"
#include "Flags/CPUAccess.h"

namespace Atom
{

	enum class TextureWrap
	{
		None = 0,
		Repeat,
		Clamp,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilter
	{
		None = 0,
		Linear,
		Nearest
	};

	enum class TextureFormat
	{
		None = 0,
		RGB,
		RGBA,
		Red,
		RedInteger,
		LuminanceAlpha,

		Depth24Stencil8,
		Depth = Depth24Stencil8,

		RedGreenBlue = RGB,
		RedGreenBlueAlpha = RGBA
	};

	struct Texture2DSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		bool FlipVertically = true;

		uint32_t MipLevels = 1;
		bool GenerateMips = false;

		uint32_t SampleCount = 1;
		uint32_t SampleQuality = 0;

		TextureFormat Format = TextureFormat::RGBA;
		TextureFilter Filter = TextureFilter::Linear;
		TextureFilter MinFilter = TextureFilter::Linear;
		TextureFilter MagFilter = TextureFilter::Linear;
		TextureWrap WrapMode = TextureWrap::Repeat;
		
		// TODO: Figure out where these should be
		Usage Usage = Usage::Default;
		CPUAccess CPUAccess = CPUAccess::None;
		BindFlags BindFlags = BindFlags::ShaderResource;
	};

}