#pragma once
#include "Atom/Graphics/Shader.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const ShaderOptions& shaderOptions);
		virtual ~DX11Shader();

		virtual void Bind() const override;
	private:
		ID3DBlob* CompileShader(const char* entryPoint, const char* target) const;
	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;
	};

}
