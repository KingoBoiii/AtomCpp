#pragma once
#include "Shader.h"
#include "BufferLayout.h"

namespace Atom
{

	enum class PrimitiveTopology
	{
		None,
		Points,
		Lines,
		Triangles
	};

	struct PipelineOptions
	{
		Shader* Shader;
		BufferLayout InputLayout;
		PrimitiveTopology PrimitiveTopology = PrimitiveTopology::Triangles;
		bool Wireframe = false;
	};

	class Pipeline
	{
	public:
		static Pipeline* Create(const PipelineOptions& pipelineOptions);
	public:
		virtual void Bind() const = 0;
	};

}
