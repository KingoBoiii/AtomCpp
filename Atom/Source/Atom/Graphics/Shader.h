#pragma once

namespace Atom
{

	struct ATOM_API ShaderOptions
	{
		std::filesystem::path Filepath;

		std::string VertexShaderEntryPoint;
		std::string VertexShaderTarget;

		std::string PixelShaderEntryPoint;
		std::string PixelShaderTarget;
	};

	class ATOM_API Shader
	{
	public:
		Shader(const ShaderOptions& shaderOptions);
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
	protected:
		ShaderOptions m_ShaderOptions;
	};

	class ATOM_API ShaderFactory
	{
	public:
		static Shader* Create(const ShaderOptions& shaderOptions);
	};

}
