#include "ATPCH.h"
#include "Shader.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11Shader.h"

namespace Atom
{

	Shader* Shader::Create(const ShaderOptions& shaderOptions)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Shader(shaderOptions);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

