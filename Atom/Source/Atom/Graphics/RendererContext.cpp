#include "ATPCH.h"
#include "RendererContext.h"
#include "Atom/Display/Window.h"

#include "DirectX11/DX11RendererContext.h"

namespace Atom
{
	RendererContext::RendererContext(Window* window)
		: m_Window(window)
	{
	}

	void RendererContext::PrintRendererContextInfo()
	{
		AT_CORE_INFO("----------------------------------------------");
		AT_CORE_INFO("Renderer API: {}", m_RendererContextInfo.RendererAPI);
		AT_CORE_INFO("Renderer: {}", m_RendererContextInfo.RendererName);
		AT_CORE_INFO("Vendor: {}", m_RendererContextInfo.VendorName);
		AT_CORE_INFO("----------------------------------------------");
	}

	RendererContext* RendererContextFactory::Create(Window* window)
	{
		return new DX11RendererContext(window);
	}
}