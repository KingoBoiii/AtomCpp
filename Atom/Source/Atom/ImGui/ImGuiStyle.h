#pragma once
#include <imgui.h>

namespace Atom::UI
{

	class ScopedStyle
	{
	public:
		template<typename T>
		ScopedStyle(ImGuiStyleVar styleVar, T value)
		{
			ImGui::PushStyleVar(styleVar, value);
		}

		~ScopedStyle()
		{
			ImGui::PopStyleVar();
		}

		ScopedStyle(const ScopedStyle&) = delete;
		ScopedStyle& operator=(const ScopedStyle&) = delete;
	};

	class ScopedFont
	{
	public:
		ScopedFont(ImFont* font)
		{
			ImGui::PushFont(font);
		}

		~ScopedFont()
		{
			ImGui::PopFont();
		}

		ScopedFont(const ScopedFont&) = delete;
		ScopedFont& operator=(const ScopedFont&) = delete;
	};

	class ScopedId
	{
	public:
		template<typename T>
		ScopedId(T id)
		{
			ImGui::PushID(id);
		}

		~ScopedId()
		{
			ImGui::PopID();
		}

		ScopedId(const ScopedId&) = delete;
		ScopedId& operator=(const ScopedId&) = delete;
	};

}
