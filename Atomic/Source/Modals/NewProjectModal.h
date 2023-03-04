#pragma once
#include <functional>
#include <string>

namespace Atom 
{

	class EditorModal
	{
	public:
		virtual ~EditorModal() = default;

		virtual void DrawUI() = 0;
		virtual void Open() = 0;
	};

	class NewProjectModal : public EditorModal
	{
	public:
		NewProjectModal() = default;
		NewProjectModal(const std::string& modalId, const std::function<void(const std::string&, const std::string&)> onCreateProjectCallback);
		virtual ~NewProjectModal();

		virtual void DrawUI() override;
		virtual void Open() override;
	private:
		std::string m_ModalId;
		std::function<void(const std::string&, const std::string&)> m_OnCreateProjectCallback;
		bool m_IsOpen = false;
	};

}
