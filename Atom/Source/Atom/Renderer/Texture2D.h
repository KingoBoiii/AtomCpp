#pragma once
#include "Texture2DSpecification.h"

#include <filesystem>

namespace Atom
{
	
	class Texture2D
	{
	public:
		static Texture2D* Create(const Texture2DSpecification& specification = Texture2DSpecification());
		static Texture2D* Create(const std::filesystem::path& filepath, const Texture2DSpecification& specification = Texture2DSpecification());
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
