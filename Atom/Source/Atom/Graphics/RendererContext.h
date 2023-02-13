#pragma once
#include "Atom/Core/Base.h"

namespace Atom
{

	struct ATOM_API RendererContextInfo
	{
		std::string RendererAPI;
		std::string VendorName;
		std::string RendererName;
	};
	
	class ATOM_API Window;

	class ATOM_API RendererContext
	{
	public:
		RendererContext(Window* window);
		virtual ~RendererContext() = default;
		
		virtual void Initialize() = 0;

		const RendererContextInfo& GetRendererContextInfo() const { return m_RendererContextInfo; }
		const RendererContextInfo& GetRendererContextInfo() { return m_RendererContextInfo; }
	protected:
		void PrintRendererContextInfo();
	protected:
		RendererContextInfo m_RendererContextInfo;
		Window* m_Window;
	};

	class ATOM_API RendererContextFactory
	{
	public:
		static RendererContext* Create(Window* window);
	};

}
