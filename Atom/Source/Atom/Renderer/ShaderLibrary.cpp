#include "ATPCH.h"
#include "ShaderLibrary.h"

namespace Atom
{

	ShaderLibrary* ShaderLibrary::Create()
	{
		return new ShaderLibrary();
	}

	void ShaderLibrary::Add(const std::string& name, Shader* shader)
	{
		AT_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Shader* ShaderLibrary::Load(const std::string& filepath)
	{
		Atom::ShaderOptions shaderOptions{ };
		shaderOptions.Filepath = filepath;
		shaderOptions.VertexShaderEntryPoint = "VSMain";
		shaderOptions.VertexShaderTarget = "vs_5_0";
		shaderOptions.PixelShaderEntryPoint = "PSMain";
		shaderOptions.PixelShaderTarget = "ps_5_0";
		Shader* shader = Shader::Create(shaderOptions);

		Add(shader->GetName(), shader);

		return shader;
	}

	Shader* ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Atom::ShaderOptions shaderOptions{ };
		shaderOptions.Filepath = filepath;
		shaderOptions.VertexShaderEntryPoint = "VSMain";
		shaderOptions.VertexShaderTarget = "vs_5_0";
		shaderOptions.PixelShaderEntryPoint = "PSMain";
		shaderOptions.PixelShaderTarget = "ps_5_0";
		Shader* shader = Shader::Create(shaderOptions);

		Add(name, shader);
		return shader;
	}

	Shader* ShaderLibrary::Get(const std::string& name)
	{
		AT_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];
	}

}