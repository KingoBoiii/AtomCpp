#pragma once

#include "Atom/Asset/AssetManager.h"

namespace Atom
{

#define ATOM_USE_LEGACY_PROJECT_SERIALIZATION 0

#define ATOM_PROJECT_CACHE_DIRECTORY "Cache"
#define ATOM_PROJECT_ASSET_REGISTRY_FILE_NAME "AssetRegistry.atre"

	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;

		// Not serialized
		std::filesystem::path ProjectFilename;
		std::filesystem::path ProjectDirectory;
	};

	class Project
	{
	public:
#if ATOM_USE_LEGACY_PROJECT_SERIALIZATION
		static Project* Create();
		static Project* Load(const std::filesystem::path& filepath);
		static bool SaveActiveProject(const std::filesystem::path& filepath);
#endif

		static const std::string& GetProjectName()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().Name;
		}

		static const std::filesystem::path& GetProjectDirectory()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_Config.ProjectDirectory;
		}

		static const std::filesystem::path& GetProjectFileName()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_Config.ProjectFilename;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return GetAssetDirectory() / path;
		}

		static std::filesystem::path GetScriptModulePath()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return GetProjectDirectory() / s_ActiveProject->GetConfig().ScriptModulePath;
		}

		static std::filesystem::path GetScriptModuleFilepath()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return GetScriptModulePath() / fmt::format("{0}.dll", GetProjectName());
		}

		static const std::filesystem::path& GetProjectCacheDirectory()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_Config.ProjectDirectory / ATOM_PROJECT_CACHE_DIRECTORY;
		}
		
		static std::filesystem::path GetAssetRegistryPath()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_Config.ProjectDirectory / ATOM_PROJECT_ASSET_REGISTRY_FILE_NAME;
		}

		static void SetActiveProject(Project* project);
		static Project* GetActiveProject() { return s_ActiveProject; }

		ProjectConfig& GetConfig() { return m_Config; }
		const ProjectConfig& GetConfig() const { return m_Config; }
	private:
		ProjectConfig  m_Config;

		inline static Project* s_ActiveProject;
	};

}
