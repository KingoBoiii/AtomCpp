#pragma once
#include "Atom/Graphics/Renderer.h"

#include "DX11RendererContext.h"
#include "DirectX11.h"

namespace Atom
{

	class DX11Renderer : public Renderer
	{
	public:
		DX11Renderer(const RendererOptions& rendererOptions);
		virtual ~DX11Renderer();
		
		virtual void Initialize() override;

		virtual void Clear() const override;
	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
	};

}
