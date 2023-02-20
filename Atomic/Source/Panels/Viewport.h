#pragma once
#include <Atom/Editor/EditorPanel.h>

#include <Atom/Renderer/Framebuffer.h>

namespace Atomic
{

	class Viewport : public Atom::EditorPanel
	{
	public:
		Viewport() = default;
		Viewport(Atom::Framebuffer* framebuffer);

		virtual void OnImGuiRender(bool& isOpen) override;

		const glm::vec2& GetViewportSize() const { return m_ViewportSize; }
	private:
		Atom::Framebuffer* m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		friend class EditorLayer;
	};

}
