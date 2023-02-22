#pragma once
#include "Atom/Renderer/Texture2D.h"

#include "DirectX11.h"

namespace Atom
{
	
	class DX11Texture2D : public Texture2D
	{
	public:
		DX11Texture2D(const std::filesystem::path& filepath, const Texture2DOptions& texture2DOptions);
		DX11Texture2D(uint32_t width, uint32_t height, const Texture2DOptions& texture2DOptions);
		virtual ~DX11Texture2D();

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void SetData(void* data) override;
		
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual void* GetTexture() const override { return static_cast<void*>(m_ShaderResourceView); }
	private:
		void Invalidate(void* data = nullptr);

		void CreateTexture(void* data, uint32_t mipLevels, D3D11_USAGE usage);
		void CreateShaderResourceView(uint32_t mipLevels);
		void CreateSamplerState();
	private:
		Texture2DOptions m_Options;
		std::filesystem::path m_Filepath;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Channels;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;

		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;
	};

}
