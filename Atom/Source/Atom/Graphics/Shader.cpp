#include "ATPCH.h"
#include "Shader.h"

#include "DirectX11/DX11Shader.h"

namespace Atom
{

	Shader::Shader(const ShaderOptions& shaderOptions)
		: m_ShaderOptions(shaderOptions)
	{
	}

	Shader* ShaderFactory::Create(const ShaderOptions& shaderOptions)
	{
		return new DX11Shader(shaderOptions);
	}

}