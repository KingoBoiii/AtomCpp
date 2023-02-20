#include "ATPCH.h"
#include "Atom/Utils/PlatformUtils.h"
#include "Atom/Core/Application.h"

#include <commdlg.h>

namespace Atom
{

	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn{ };
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = (HWND)Application::Get().GetWindow()->GetNativeWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn))
		{
			return ofn.lpstrFile;
		}
		
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn{ };
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = (HWND)Application::Get().GetWindow()->GetNativeWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if(GetSaveFileNameA(&ofn))
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

}