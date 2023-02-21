#pragma once
#include "Atom/Display/Window.h"

namespace Atom
{

	class Win32Window : public Window
	{
	public:
		Win32Window(const WindowOptions& windowOptions);
		virtual ~Win32Window();

		virtual void Initialize() override;
		virtual void Update() const override;
		virtual void Present() const override;

		virtual void Maximize() override;
		virtual void CenterWindow() override;

		virtual void SetEventCallback(const EventCallbackFn& callback) { m_WindowData.EventCallback = callback; }

		inline virtual uint32_t GetWidth() const { return m_WindowData.Width; }
		inline virtual uint32_t GetHeight() const { return m_WindowData.Height; }
		virtual void* GetNativeWindowHandle() const override { return static_cast<void*>(m_WindowHandle); }
	private:
		HWND m_WindowHandle = nullptr;
		HINSTANCE m_HInstance = nullptr;

		struct WindowData
		{
			uint32_t Width;
			uint32_t Height;
			EventCallbackFn EventCallback;
		};
		WindowData m_WindowData;
	private:
		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

}

