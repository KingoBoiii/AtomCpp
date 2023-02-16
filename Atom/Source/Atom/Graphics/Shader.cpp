#include "ATPCH.h"
#include "Shader.h"

namespace Atom
{

	Shader::Shader(const ShaderOptions& shaderOptions)
		: m_ShaderOptions(shaderOptions)
	{
	}

	Shader* ShaderFactory::Create(const ShaderOptions& shaderOptions)
	{
		return nullptr;
	}

}