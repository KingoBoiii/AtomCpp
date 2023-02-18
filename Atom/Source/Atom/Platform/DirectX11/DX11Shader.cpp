#include "ATPCH.h"
#include "DX11Shader.h"
#include "DX11RendererContext.h"

namespace Atom
{

	DX11Shader::DX11Shader(const ShaderOptions& shaderOptions)
		: m_Options(shaderOptions)
	{
		DX11RendererContext& context = DX11RendererContext::Get();
		m_Device = context.m_Device;
		m_DeviceContext = context.m_DeviceContext;

		m_VertexSourceBlob = CompileShader(m_Options.VertexShaderEntryPoint.c_str(), m_Options.VertexShaderTarget.c_str());
		ID3DBlob* pixelShaderBlob = CompileShader(m_Options.PixelShaderEntryPoint.c_str(), m_Options.PixelShaderTarget.c_str());

		// Create vertex shader
		HRESULT hr = m_Device->CreateVertexShader(
			m_VertexSourceBlob->GetBufferPointer(),
			m_VertexSourceBlob->GetBufferSize(),
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

		ReleaseCOM(pixelShaderBlob);
	}

	DX11Shader::~DX11Shader()
	{
		ReleaseCOM(m_VertexSourceBlob);
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
		ID3DBlob* errorBlob = nullptr;

		HRESULT hr = D3DCompileFromFile(
			m_Options.Filepath.c_str(),
			nullptr,
			nullptr,
			entryPoint,
			target,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&shaderBlob,
			&errorBlob
		);

		AT_CORE_ASSERT(SUCCEEDED(hr), (char*)errorBlob->GetBufferPointer());

		return shaderBlob;
	}

}