project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"Source/**.cpp",
		"Source/**.h"
	}
	
	includedirs {
		"%{wks.location}/Atom/Source",
        "%{IncludeDir.spdlog}"
    }

	links {
		"Atom"
	}

    defines {
        "ATOM_STATIC_LIB"
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