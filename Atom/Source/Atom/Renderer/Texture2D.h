#pragma once
#include <filesystem>

namespace Atom
{

	enum class TextureWrap
	{
		NONE = 0,
		REPEAT,
		CLAMP,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class TextureFilter
	{
		NONE = 0,
		LINEAR,
		NEAREST
	};

	enum class TextureFormat
	{
		NONE = 0,
		RGB,
		RGBA,
		RED,
		RED_INTEGER,
		LUMINANCE_ALPHA
	};

	struct Texture2DOptions
	{
		TextureFormat Format = TextureFormat::RGBA;
		TextureFilter Filter = TextureFilter::LINEAR;
		TextureFilter MinFilter = TextureFilter::LINEAR;
		TextureFilter MagFilter = TextureFilter::LINEAR;
		TextureWrap WrapMode = TextureWrap::REPEAT;
		bool FlipVertically = true;
	};

	class Texture2D
	{
	public:
		static Texture2D* Create(const std::filesystem::path& filepath, const Texture2DOptions& texture2DOptions = Texture2DOptions());
		static Texture2D* Create(uint32_t width, uint32_t height, const Texture2DOptions& texture2DOptions = Texture2DOptions());
	public:
		virtual void Bind(uint32_t slot = 0) const = 0;
		
		virtual void SetData(void* data) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void* GetTexture() const = 0;
	};

}
