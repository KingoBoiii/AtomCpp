#pragma once

#include "Atom/Renderer/Texture2D.h"

namespace Atom
{

	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& filepath);
		~Font();

		MSDFData* GetMSDFData() const { return m_Data; }
		Texture2D* GetAtlasTexture() const { return m_AtlasTexture; }

		static Font* GetDefaultFont();
	private:
		MSDFData* m_Data = nullptr;
		Texture2D* m_AtlasTexture = nullptr;
	};

}
