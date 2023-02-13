workspace "Atom"
	architecture "x86_64"
	startproject "Sandbox"

	configurations { 
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir['spdlog'] = "%{wks.location}/Atom/vendor/spdlog/include";

LibraryDir = {}
Library = {}

group "Dependencies"
    include "Atom/Vendor/spdlog"
group ""

include "Atom"

include "Sandbox"

