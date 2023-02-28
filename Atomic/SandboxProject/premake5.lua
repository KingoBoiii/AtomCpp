AtomRootDir = os.getenv("ATOM_DIR")
include (AtomRootDir .. "/Vendor/premake/premake_customization/solution_items.lua")

workspace "Sandbox"
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

project "Sandbox"
	kind "SharedLib"
	namespace "Sandbox"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Cache/Scripts/Binaries")
	objdir ("Cache/Scripts/Intermediates")

	files {
		"Assets/**.cs"
	}

	links {
		"Atom.Core"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

group "Atom"
	include (AtomRootDir .. "/Atom.Core")
group ""