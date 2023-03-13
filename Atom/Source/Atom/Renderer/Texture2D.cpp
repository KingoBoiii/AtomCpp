#include "ATPCH.h"
#include "Texture2D.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11Texture2D.h"

namespace Atom
{

	Texture2D* Texture2D::Create(const Texture2DSpecification& specification)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Texture2D(specification);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Texture2D* Texture2D::Create(const std::filesystem::path& filepath, const Texture2DSpecification& specification)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Texture2D(filepath, specification);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	//Texture2D* Texture2D::Create(uint32_t width, uint32_t height, const Texture2DSpecification& specification)
	//{
	//	switch(Renderer::Current())
	//	{
	//		case RendererAPI::DirectX11:	return new DX11Texture2D(width, height, specification);
	//		case RendererAPI::OpenGL:
	//		case RendererAPI::Unknown:
	//		default: break;
	//	}

	//	AT_CORE_ASSERT(false, "Unknown RendererAPI!");
	//	return nullptr;
	//}

}