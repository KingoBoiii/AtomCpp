#pragma once
#include "Project.h"

namespace Atom
{

	class ProjectSerializer
	{
	public:
		ProjectSerializer(Project* project);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	private:
		Project* m_Project;
	};

}