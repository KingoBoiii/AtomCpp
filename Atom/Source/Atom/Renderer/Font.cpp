#include "ATPCH.h"
#include "Font.h"
#include "MSDFData.h"

#undef INFINITE
#include <msdf-atlas-gen.h>
#include <FontGeometry.h>

namespace Atom
{

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Texture2D* CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const msdf_atlas::FontGeometry& fontGeometry
										  , uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		Texture2DSpecification spec;
		spec.Width = width;
		spec.Height = height;
		Texture2D* texture = Texture2D::Create(spec);
		texture->SetData((void*)bitmap.pixels);
		return texture;
	}

	Font::Font(const std::filesystem::path& filepath)
		: m_Data(new MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		AT_CORE_ASSERT(ft);

		std::string fileString = filepath.string();
		msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str()); //filepath.string().c_str() "C:\\Windows\\Fonts\\arialbd.ttf"
		if(!font)
		{
			AT_CORE_ERROR("Failed to load font: {}", fileString);
			return;
		}

		struct CharsetRange
		{
			uint32_t Begin, End;
		};

		// From imgui_draw.cpp
		static const CharsetRange charsetRange[] = {
			{ 0x0020, 0x00FF }
		};

		msdf_atlas::Charset charset;
		for(CharsetRange range : charsetRange)
		{
			for(uint32_t c = range.Begin; c <= range.End; c++)
			{
				charset.add(c);
			}
		}

		double fontScale = 1.0;
		m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
		int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
		AT_CORE_INFO("Loading {} glyphs from font (out of {})", glyphsLoaded, charset.size());

		double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		//atlasPacker.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::EVEN_SQUARE);
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0.0);
		atlasPacker.setScale(emSize);
		int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
		AT_CORE_ASSERT(remaining == 0);

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8

		// if MSDF || MTSDF 
		// Edge coloring
		uint64_t coloringSeed = 0;
		bool expensiveColoring = false;
		if(expensiveColoring)
		{
			msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool
			{
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
			}, m_Data->Glyphs.size()).finish(THREAD_COUNT);
		}
		else
		{
			unsigned long long glyphSeed = coloringSeed;
			for(msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
			{
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}

		m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 4, msdf_atlas::mtsdfGenerator>("Test", 0.0f, m_Data->Glyphs, m_Data->FontGeometry, width, height);


#if 0
		msdfgen::Shape shape;
		if(msdfgen::loadGlyph(shape, font, 'C'))
		{
			shape.normalize();

			msdfgen::edgeColoringSimple(shape, 3.0);

			msdfgen::Bitmap<float, 3> msdf(32, 32);

			msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));

			msdfgen::savePng(msdf, "output.png");
	}
#endif

		msdfgen::destroyFont(font);

		msdfgen::deinitializeFreetype(ft);
}

	Font::~Font()
	{
		delete m_Data;
	}

	Font* Font::GetDefaultFont()
	{
		static Font* DefaultFont;
		if(!DefaultFont)
		{
			DefaultFont = new Font("Resources/Fonts/OpenSans/OpenSans-Regular.ttf");
		}

		return DefaultFont;
	}

}