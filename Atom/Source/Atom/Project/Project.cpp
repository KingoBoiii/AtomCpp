#include "ATPCH.h"
#include "Project.h"
#include "ProjectSerializer.h"

namespace Atom
{

#if ATOM_USE_LEGACY_PROJECT_SERIALIZATION
	Project* Project::Create()
	{
		s_ActiveProject = new Project();
		return s_ActiveProject;
	}

	Project* Project::Load(const std::filesystem::path& filepath)
	{
		Project* project = new Project();

		ProjectSerializer serializer(project);
		if(serializer.Deserialize(filepath))
		{
			project->m_ProjectDirectory = filepath.parent_path();
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActiveProject(const std::filesystem::path& filepath)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if(serializer.Serialize(filepath))
		{
			s_ActiveProject->m_ProjectDirectory = filepath.parent_path();
			return true;
		}

		return false;
	}
#endif 

	void Project::SetActiveProject(Project* project)
	{
		s_ActiveProject = project; 

		if(!s_ActiveProject)
		{
			return;
		}
		AssetManager::Initialize();
	}

}