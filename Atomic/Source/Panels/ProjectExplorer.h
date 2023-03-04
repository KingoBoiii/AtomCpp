#pragma once

#include <Atom/Editor/EditorPanel.h>

namespace Atom
{
	
	class ProjectExplorer : public EditorPanel
	{
	public:
		ProjectExplorer();
		~ProjectExplorer() = default;

		virtual void DrawUI(bool& isOpen) override;
		virtual void OnProjectChanged(Project* project) override;
	private:
		void DrawAssetDirectoryTree();
		void DrawDirectoryTree(const std::filesystem::path& directoryPath);
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
	};

}
