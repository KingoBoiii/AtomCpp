#include "ATPCH.h"
#include "DX11Shader.h"

#include "DX11RendererContext.h"

namespace Atom
{

	DX11Shader::DX11Shader(const ShaderOptions& shaderOptions)
		: Shader(shaderOptions)
	{
		DX11RendererContext context = DX11RendererContext::Get();

		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		ID3DBlob* vertexShaderBlob = CompileShader(m_ShaderOptions.VertexShaderEntryPoint.c_str(), m_ShaderOptions.VertexShaderTarget.c_str());
		ID3DBlob* pixelShaderBlob = CompileShader(m_ShaderOptions.PixelShaderEntryPoint.c_str(), m_ShaderOptions.PixelShaderTarget.c_str());
		
		// Create vertex shader
		HRESULT hr = m_Device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			nullptr,
			&m_VertexShader
		);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create vertex shader!");

		// Create pixel shader
		hr = m_Device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(),
			nullptr,
			&m_PixelShader
		);
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to create pixel shader!");

		vertexShaderBlob->Release();
		pixelShaderBlob->Release();
	}

	DX11Shader::~DX11Shader()
	{
		ReleaseCOM(m_VertexShader);
		ReleaseCOM(m_PixelShader);
	}

	void DX11Shader::Bind() const
	{
		m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
		m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
	}

	ID3DBlob* DX11Shader::CompileShader(const char* entryPoint, const char* target) const
	{
		ID3DBlob* shaderBlob = nullptr;

		HRESULT hr = D3DCompileFromFile(
			m_ShaderOptions.Filepath.c_str(),
			nullptr,
			nullptr,
			entryPoint,
			target,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&shaderBlob,
			nullptr
		);
		
		AT_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader");

		return shaderBlob;
	}

}