#pragma once
#include "Atom/Graphics/RendererContext.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11RendererContext : public RendererContext
	{
	public:
		DX11RendererContext(Window* window);
		virtual ~DX11RendererContext();

		virtual void Initialize() override;

		inline static DX11RendererContext& Get() { return *s_Instance; }
	private:
		void CreateRendererContextInfo();
	private:
		ID3D11Device4* m_Device = nullptr;
		ID3D11DeviceContext4* m_DeviceContext = nullptr;
		D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	private:
		static DX11RendererContext* s_Instance;

		friend class DX11SwapChain;
		friend class DX11Renderer;
		friend class DX11VertexBuffer;
		friend class DX11IndexBuffer;
		friend class DX11Shader;
		friend class DX11Pipeline;
		friend class DX11UniformBuffer;
	};

}
