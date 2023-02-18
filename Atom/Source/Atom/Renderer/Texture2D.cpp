#include "ATPCH.h"
#include "Texture2D.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11Texture2D.h"

namespace Atom
{

	Texture2D* Texture2D::Create(const std::filesystem::path& filepath, const Texture2DOptions& texture2DOptions)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Texture2D(filepath, texture2DOptions);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}