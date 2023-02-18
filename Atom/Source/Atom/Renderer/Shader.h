#pragma once

#include <filesystem>

namespace Atom
{

	struct ShaderOptions
	{
		std::filesystem::path Filepath;

		std::string VertexShaderEntryPoint;
		std::string VertexShaderTarget;

		std::string PixelShaderEntryPoint;
		std::string PixelShaderTarget;
	};

	class Shader
	{
	public:
		static Shader* Create(const ShaderOptions& shaderOptions);
	public:
		virtual void Bind() const = 0;
	};

}
