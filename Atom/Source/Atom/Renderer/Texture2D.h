#pragma once
#include <filesystem>

namespace Atom
{

	enum class TextureWrap
	{
		NONE = 0,
		Repeat,
		Clamp,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilter
	{
		NONE = 0,
		Linear,
		Nearest
	};

	enum class TextureFormat
	{
		NONE = 0,
		RGB,
		RGBA,
		Red,
		RedInteger,
		LuminanceAlpha,

		RedGreenBlue = RGB,
		RedGreenBlueAlpha = RGBA
	};

	enum class TextureUsage
	{
		NONE = 0,
		Static,
		Default = Static,
		Dynamic,
		Staging
	};

	struct Texture2DSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		bool FlipVertically = true;
		uint32_t MipLevels = 1;

		bool RenderTarget = false;

		uint32_t SampleCount = 1;
		uint32_t SampleQuality = 0;

		TextureFormat Format = TextureFormat::RGBA;
		TextureFilter Filter = TextureFilter::Linear;
		TextureFilter MinFilter = TextureFilter::Linear;
		TextureFilter MagFilter = TextureFilter::Linear;
		TextureWrap WrapMode = TextureWrap::Repeat;
		TextureUsage Usage = TextureUsage::Default;
	};

	class Texture2D
	{
	public:
		static Texture2D* Create(const Texture2DSpecification& specification = Texture2DSpecification());
		static Texture2D* Create(const std::filesystem::path& filepath, const Texture2DSpecification& specification = Texture2DSpecification());
		//static Texture2D* Create(uint32_t width, uint32_t height, const Texture2DSpecification& specification = Texture2DSpecification());
	public:
		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void SetData(void* data) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const Texture2DSpecification& GetSpecification() = 0;
		virtual const Texture2DSpecification& GetSpecification() const = 0;
		virtual void* GetTextureHandle() const = 0;
	};

}
