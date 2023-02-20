#pragma once
#include "Shader.h"

namespace Atom
{

	class ShaderLibrary
	{
	public:
		static ShaderLibrary* Create();
	public:
		ShaderLibrary() = default;
		~ShaderLibrary() = default;

		void Add(const std::string& name, Shader* shader);
		
		Shader* Load(const std::string& filepath);
		Shader* Load(const std::string& name, const std::string& filepath);

		Shader* Get(const std::string& name);
	private:
		std::unordered_map<std::string, Shader*> m_Shaders;
	};

}
