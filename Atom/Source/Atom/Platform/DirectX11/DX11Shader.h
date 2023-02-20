#pragma once
#include "Atom/Renderer/Shader.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const ShaderOptions& shaderOptions);
		~DX11Shader();

		virtual void Bind() const override;

		virtual const std::string& GetName() const override { return m_Name; }
	private:
		void Invalidate();
		ID3DBlob* CompileShader(const char* entryPoint, const char* target) const;

		void GetNameByFilepath(const std::string& filepath);
	private:
		ShaderOptions m_Options;
		std::string m_Name;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;

		ID3DBlob* m_VertexSourceBlob = nullptr;

		friend class DX11Pipeline;
	};

}
