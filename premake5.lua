workspace "Atom"
	architecture "x86_64"
	startproject "Sandbox"

	configurations { 
		"Debug",
		"Release"
	}

    flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir['spdlog'] = "%{wks.location}/Atom/vendor/spdlog/include";
IncludeDir["GLFW"] = "%{wks.location}/Atom/Vendor/glfw/include";
IncludeDir["glm"] = "%{wks.location}/Atom/Vendor/glm";
IncludeDir["entt"] = "%{wks.location}/Atom/Vendor/entt/include";
IncludeDir["mono"] = "%{wks.location}/Atom/Vendor/mono/include";

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Atom/Vendor/mono/lib/%{cfg.buildcfg}";

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"

group "Dependencies"
    include "Atom/Vendor/spdlog"
    include "Atom/vendor/glfw"
group ""

include "Atom"

include "Sandbox"

