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

	struct ATOM_API PipelineOptions
	{
		Shader* Shader;
		BufferLayout InputLayout;
		PrimitiveTopology PrimitiveTopology = PrimitiveTopology::Triangles;
		bool Wireframe = false;
	};

	class ATOM_API Pipeline
	{
	public:
		Pipeline(const PipelineOptions& pipelineOptions);
		virtual ~Pipeline() = default;

		virtual void Bind() const = 0;
	protected:
		PipelineOptions m_PipelineOptions;
	};

	class ATOM_API PipelineFactory
	{
	public:
		static Pipeline* Create(const PipelineOptions& pipelineOptions);
	};

}
