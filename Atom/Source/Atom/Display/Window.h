#pragma once
#include "Atom/Core/Base.h"
#include "WindowOptions.h"
#include "Atom/Events/Event.h"

namespace Atom
{

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

		const WindowOptions& GetWindowOptions() const { return m_WindowOptions; }
		const WindowOptions& GetWindowOptions() { return m_WindowOptions; }
	protected:
		WindowOptions m_WindowOptions;
	};

}
