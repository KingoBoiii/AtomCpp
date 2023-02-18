#pragma once
#include "Atom/Core/Base.h"
#include "WindowOptions.h"
#include "Atom/Events/Event.h"

namespace Atom
{

	class ATOM_API RendererContext;
	class ATOM_API SwapChain;

	class ATOM_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
	public:
		Window() = delete;
		Window(const Window&) = delete;
		Window(const WindowOptions& windowOptions);
		virtual ~Window() = default;

		virtual void Initialize() = 0;
		virtual void Update() const = 0;
		virtual void Present() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void* GetNativeWindowHandle() const = 0;

		SwapChain* GetSwapChain() const { return m_SwapChain; }

		virtual float GetAspectRatio() const { return (float)m_WindowOptions.Width / (float)m_WindowOptions.Height; }

		const WindowOptions& GetWindowOptions() const { return m_WindowOptions; }
		const WindowOptions& GetWindowOptions() { return m_WindowOptions; }
	protected:
		WindowOptions m_WindowOptions;
		RendererContext* m_RendererContext;
		SwapChain* m_SwapChain;
	};

}
