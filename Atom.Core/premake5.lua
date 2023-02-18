project "Atom.Core"
    namespace "Atom"
	kind "SharedLib"
	language "C#"
    dotnetframework "4.7.2"

	targetdir ("../Atomic/Resources/Scripts")
	objdir ("../Atomic/Resources/Scripts/Intermediates")

	files {
		"**.cs",
		"Properties/**.cs"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
        optimize "On"
        symbols "Default"