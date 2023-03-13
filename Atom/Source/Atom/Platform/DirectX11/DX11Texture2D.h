#pragma once
#include "Atom/Renderer/Texture2D.h"

#include "DirectX11.h"

namespace Atom
{
	
	class DX11Texture2D : public Texture2D
	{
	public:
		DX11Texture2D(const Texture2DSpecification& specification);
		DX11Texture2D(const std::filesystem::path& filepath, const Texture2DSpecification& texture2DOptions);
		virtual ~DX11Texture2D();

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void SetData(void* data) override;
		
		virtual uint32_t GetWidth() const override { return m_Specification.Width; }
		virtual uint32_t GetHeight() const override { return m_Specification.Height; }
		virtual const Texture2DSpecification& GetSpecification() override { return m_Specification; }
		virtual const Texture2DSpecification& GetSpecification() const override { return m_Specification; }
		virtual void* GetTextureHandle() const override { return static_cast<void*>(m_ShaderResourceView); }
	private:
		void Invalidate(void* data = nullptr);

		void CreateTexture(void* data);
		void CreateShaderResourceView();
		void CreateSamplerState();
	private:
		Texture2DSpecification m_Specification;
		std::filesystem::path m_Filepath;
		uint32_t m_Channels;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;

		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;

		friend class DX11Framebuffer;
	};

}
