#pragma once
#include "Atom/Graphics/Pipeline.h"

#include "DirectX11.h"

namespace Atom
{

	class DX11Pipeline : public Pipeline
	{
	public:
		DX11Pipeline(const PipelineOptions& pipelineOptions);
		virtual ~DX11Pipeline();

		virtual void Bind() const override;
	private:
		void Invalidate();
		void CreateRasterizerState();
	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;

		ID3D11InputLayout* m_InputLayout = nullptr;
		ID3D11RasterizerState* m_RasterizerState = nullptr;
	};

}
