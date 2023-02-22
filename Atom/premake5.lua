project "Atom"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ATPCH.h"
	pchsource "Source/ATPCH.cpp"

	files {
		"Source/**.cpp",
		"Source/**.h",
        "Vendor/glm/glm/**.hpp",
		"Vendor/glm/glm/**.inl",

		"Vendor/ImGuizmo/ImGuizmo.h",
		"Vendor/ImGuizmo/ImGuizmo.cpp"
	}
    
    includedirs {
        "Source",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.FileWatch}"
    }

    links {
        "spdlog",
        "GLFW",
        "ImGui",
        "yaml-cpp",
        "Box2D",
        "%{Library.mono}",
        "opengl32.lib"
    }

	defines {
        "ATOM_STATIC_LIB",
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	filter "files:Vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

        links {
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}

	filter "configurations:Debug"
		defines "AT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AT_RELEASE"
		runtime "Release"
		optimize "on"
