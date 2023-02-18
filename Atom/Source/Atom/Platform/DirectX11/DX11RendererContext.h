#pragma once
#include "Atom/Renderer/RendererContext.h"

#include <d3d11_4.h>

#define ReleaseCOM(x)	\
	if (x)				\
	{					\
		x->Release();	\
		x = nullptr;		\
	}

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
		Window* m_Window = nullptr;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

		friend class DX11SwapChain;
		friend class DX11Renderer;
		friend class DX11Pipeline;
		friend class DX11VertexBuffer;
		friend class DX11IndexBuffer;
		friend class DX11UniformBuffer;
		friend class DX11Shader;
		friend class ImGuiLayer;
	private:
		static DX11RendererContext* s_Instance;
	};

}
