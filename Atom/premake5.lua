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
		"Source/**.h"
	}
    
    includedirs {
        "Source",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}"
    }

    links {
        "spdlog",
        "GLFW"
    }

	defines {
        "ATOM_STATIC_LIB",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "AT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AT_RELEASE"
		runtime "Release"
		optimize "on"
