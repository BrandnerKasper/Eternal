workspace "Eternal"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (in our case solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Eternal/vendor/GLFW/include"
IncludeDir["Glad"] = "Eternal/vendor/Glad/include"
IncludeDir["ImGui"] = "Eternal/vendor/imgui"

include "Eternal/vendor/GLFW"
include "Eternal/vendor/Glad"
include "Eternal/vendor/imgui"

project "Eternal"
	location "Eternal"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ETpch.h"
	pchsource "Eternal/src/ETpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off" -- if this leads to problems turn to on and use those buildoptions
		systemversion "latest"

		defines
		{
			"ET_PLATFORM_WINDOWS",
			"ET_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "ET_DEBUG"
		--buildoptions " /MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ET_RELEASE"
		--buildoptions " /MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ET_DIST"
		--buildoptions " /MD"
		optimize "On"

-------------------------------------------------------------------------------------------

project "Sandbox" 
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Eternal/vendor/spdlog/include",
		"Eternal/src"
	}

	links
	{
		"Eternal"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"ET_PLATFORM_WINDOWS",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ET_DEBUG"
		--buildoptions " /MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ET_RELEASE"
		--buildoptions " /MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ET_DIST"
		--buildoptions " /MD"
		optimize "On"