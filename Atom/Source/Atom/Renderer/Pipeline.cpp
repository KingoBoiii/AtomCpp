#include "ATPCH.h"
#include "Pipeline.h"
#include "Renderer.h"

#include "Atom/Platform/DirectX11/DX11Pipeline.h"

namespace Atom
{

	Pipeline* Pipeline::Create(const PipelineOptions& pipelineOptions)
	{
		switch(Renderer::Current())
		{
			case RendererAPI::DirectX11:	return new DX11Pipeline(pipelineOptions);
			case RendererAPI::OpenGL:
			case RendererAPI::Unknown:
			default: break;
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}