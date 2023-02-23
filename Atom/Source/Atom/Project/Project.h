#pragma once

namespace Atom
{

	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};

	class Project
	{
	public:
		static Project* Create();
		static Project* Load(const std::filesystem::path& filepath);
		static bool SaveActiveProject(const std::filesystem::path& filepath);

		static const std::string& GetProjectName()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().Name;
		}

		static const std::filesystem::path& GetProjectDirectory()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectDirectory;
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
			return GetAssetDirectory() / s_ActiveProject->GetConfig().ScriptModulePath;
		}

		static std::filesystem::path GetScriptModuleFilepath()
		{
			AT_CORE_ASSERT(s_ActiveProject);
			return GetScriptModulePath() / fmt::format("{0}.dll", GetProjectName());
		}

		static Project* GetActiveProject() { return s_ActiveProject; }

		ProjectConfig& GetConfig() { return m_Config; }
		const ProjectConfig& GetConfig() const { return m_Config; }
	private:
		ProjectConfig  m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Project* s_ActiveProject;
	};

}
