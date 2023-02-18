#include "ATPCH.h"
#include "Win32Window.h"

#include "Atom/Renderer/RendererContext.h"
#include "Atom/Renderer/SwapChain.h"

#include "Atom/Core/MouseButtons.h"

#include "Atom/Events/WindowEvent.h"
#include "Atom/Events/MouseEvent.h"
#include "Atom/Events/KeyEvent.h"

#include <Windows.h>

#include <backends/imgui_impl_win32.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace Atom
{

	static Win32Window* s_Window = nullptr;
	static const wchar_t* s_ClassName = L"Atom Win32 Window";

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		{
			return true;
		}

		LRESULT result = NULL;

		switch(message)
		{
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent e((uint32_t)Atom::MouseButton::Left);
				s_Window->m_WindowData.EventCallback(e);
			} break;
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent e((uint32_t)Atom::MouseButton::Right);
				s_Window->m_WindowData.EventCallback(e);
			} break;
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent e((uint32_t)Atom::MouseButton::Middle);
				s_Window->m_WindowData.EventCallback(e);
			} break;
			case WM_MOUSEWHEEL:
			{
				int zDelta = GET_WHEEL_DELTA_WPARAM(wParam) / 100;
				MouseScrolledEvent e(0.0f, static_cast<float>(zDelta));
				s_Window->m_WindowData.EventCallback(e);
			} break;
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				uint32_t flags = static_cast<uint32_t>(lParam);
				uint32_t key = static_cast<uint32_t>(wParam);
				KeyReleasedEvent e(key);
				s_Window->m_WindowData.EventCallback(e);
			} break;
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				uint32_t flags = static_cast<uint32_t>(lParam);
				uint32_t key = static_cast<uint32_t>(wParam);
				KeyPressedEvent e(key, (flags >> 30) & 1);
				s_Window->m_WindowData.EventCallback(e);
			} break;
			case WM_SIZE:		// Window Resize event
			{
				s_Window->m_WindowData.Width = LOWORD(lParam);
				s_Window->m_WindowData.Height = HIWORD(lParam);

				WindowResizeEvent e(s_Window->m_WindowData.Width, s_Window->m_WindowData.Height);
				s_Window->m_WindowData.EventCallback(e);
			} break;
			case WM_CLOSE:		// Window Close event
			{
				WindowCloseEvent e;
				s_Window->m_WindowData.EventCallback(e);
				PostQuitMessage(0);
			} break;
			default: result = DefWindowProc(hWnd, message, wParam, lParam); break;
		}
		return result;
	}

	Win32Window::Win32Window(const WindowOptions& windowOptions)
		: Window(windowOptions)
	{
		m_HInstance = (HINSTANCE)&__ImageBase;

		s_Window = this;
	}

	Win32Window::~Win32Window()
	{
		DestroyWindow(m_WindowHandle);
		UnregisterClassW(s_ClassName, m_HInstance);

		s_Window = nullptr;
	}

	void Win32Window::Initialize()
	{
		WNDCLASSEX wclass = { 0 }; // Or: WNDCLASSEXW
		wclass.cbSize = sizeof(wclass);
		wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wclass.lpfnWndProc = &WndProc;
		wclass.cbClsExtra = 0;
		wclass.cbWndExtra = 0;
		wclass.hInstance = m_HInstance;
		wclass.hIcon = NULL; // TODO: CREATE ICON
		wclass.hCursor = NULL;
		wclass.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
		wclass.lpszMenuName = NULL;
		wclass.lpszClassName = s_ClassName;
		wclass.hIconSm = NULL;

		ATOM result = RegisterClassEx(&wclass);
		AT_CORE_ASSERT(SUCCEEDED(result), "Failed to register Win32 class");

		std::wstring wTitle = std::wstring(m_WindowOptions.Title.begin(), m_WindowOptions.Title.end());
		m_WindowHandle = CreateWindowW( // Or: CreateWindowW()
									   s_ClassName,
									   wTitle.c_str(), // Or: L"NAME OF WINDOW"
									   WS_VISIBLE | WS_OVERLAPPEDWINDOW,
									   CW_USEDEFAULT,
									   CW_USEDEFAULT,
									   m_WindowOptions.Width,//WIDTH:[TODO]->Make custom width to fit window
									   m_WindowOptions.Height,//HEIGHT:[TODO]->Make custom width to fit window
									   0,
									   0,
									   m_HInstance,
									   0
		);
		AT_CORE_ASSERT(m_WindowHandle, "Failed to create Win32 window");

		m_RendererContext = RendererContext::Create(this);
		m_RendererContext->Initialize();

		m_SwapChain = SwapChain::Create(this);
		m_SwapChain->Initialize();

		// Show the window
		ShowWindow(m_WindowHandle, SW_SHOW);
		UpdateWindow(m_WindowHandle);
	}

	void Win32Window::Update() const
	{
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Win32Window::Present() const
	{
		m_SwapChain->Present();
	}

}

