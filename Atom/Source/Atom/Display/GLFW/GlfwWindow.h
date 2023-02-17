#pragma once
#include "Atom/Display/Window.h"

struct GLFWwindow;

namespace Atom
{

	class GlfwWindow : public Window
	{
	public:
		GlfwWindow(const WindowOptions& windowOptions);
		virtual ~GlfwWindow();

		virtual void Initialize() override;
		virtual void Update() const override;
		virtual void Present() const override;

		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; }

		virtual void* GetNativeWindowHandle() const override;
	private:
		void SetupCallbacks();
	private:
		GLFWwindow* m_WindowHandle = nullptr;

		struct WindowData
		{
			EventCallbackFn EventCallback;
		};
		WindowData m_WindowData;

		friend class Input;
		friend class ImGuiLayer;
	};

}
