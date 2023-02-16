#include "ATPCH.h"
#include "Pipeline.h"

#include "DirectX11/DX11Pipeline.h"

namespace Atom
{

	Pipeline::Pipeline(const PipelineOptions& pipelineOptions)
		: m_PipelineOptions(pipelineOptions)
	{
	}

	Pipeline* PipelineFactory::Create(const PipelineOptions& pipelineOptions)
	{
		return new DX11Pipeline(pipelineOptions);
	}

}