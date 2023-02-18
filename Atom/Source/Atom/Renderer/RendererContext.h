#pragma once
#include "Atom/Core/Base.h"

namespace Atom
{

	class Window;

	struct ATOM_API RendererContextInfo
	{
		std::string RendererAPI;
		std::string VendorName;
		std::string RendererName;
	};

	class RendererContext
	{
	public:
		static RendererContext* Create(Window* window);
	public:
		virtual void Initialize() = 0;
		
		const RendererContextInfo& GetContextInfo() { return m_RendererContextInfo; }
		const RendererContextInfo& GetContextInfo() const { return m_RendererContextInfo; }
	protected:
		RendererContextInfo m_RendererContextInfo;
	};

}
