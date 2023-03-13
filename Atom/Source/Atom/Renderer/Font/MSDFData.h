#pragma once

#undef INFINITE
#include <FontGeometry.h>

namespace Atom
{
	
	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry FontGeometry;
	};
}