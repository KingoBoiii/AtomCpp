workspace "Atom"
	architecture "x86_64"
	startproject "Atomic"

	configurations { 
		"Debug",
		"Release"
	}

    flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir['spdlog'] = "%{wks.location}/Atom/Vendor/spdlog/include";
IncludeDir["GLFW"] = "%{wks.location}/Atom/Vendor/glfw/include";
IncludeDir["glm"] = "%{wks.location}/Atom/Vendor/glm";
IncludeDir["entt"] = "%{wks.location}/Atom/Vendor/entt/include";
IncludeDir["mono"] = "%{wks.location}/Atom/Vendor/mono/include";
IncludeDir["ImGui"] = "%{wks.location}/Atom/Vendor/imgui";
IncludeDir["stb"] = "%{wks.location}/Atom/Vendor/stb/include";
IncludeDir["yaml_cpp"] = "%{wks.location}/Atom/Vendor/yaml-cpp/include";
IncludeDir["ImGuizmo"] = "%{wks.location}/Atom/Vendor/ImGuizmo";
IncludeDir["Box2D"] = "%{wks.location}/Atom/Vendor/Box2D/include";
IncludeDir["FileWatch"] = "%{wks.location}/Atom/Vendor/filewatch";
IncludeDir["msdfgen"] = "%{wks.location}/Atom/Vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Atom/Vendor/msdf-atlas-gen/msdf-atlas-gen"

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
    include "Atom/Vendor/glfw"
    include "Atom/Vendor/imgui"
    include "Atom/Vendor/yaml-cpp"
    include "Atom/Vendor/Box2D"
    include "Atom/Vendor/msdf-atlas-gen"
group ""

group "Core" 
    include "Atom"
    include "Atom.Core"
group ""
    
group "Tools"
    include "Atomic"
group ""

include "Sandbox"

