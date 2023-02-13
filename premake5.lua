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
IncludeDir["GLFW"] = "%{wks.location}/Atom/Vendor/glfw/include"

LibraryDir = {}
Library = {}

group "Dependencies"
    include "Atom/Vendor/spdlog"
    include "Atom/vendor/glfw"
group ""

include "Atom"

include "Sandbox"

