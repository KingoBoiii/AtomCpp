#pragma once

#include <filesystem>

namespace Atom
{

	struct ShaderSpecification
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
		static Shader* Create(const ShaderSpecification& specification);
	public:
		virtual void Bind() const = 0;

		virtual const std::string& GetName() const = 0;
	};

}

#include "ShaderLibrary.h"
