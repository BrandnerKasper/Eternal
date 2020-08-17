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
IncludeDir["glm"] = "Eternal/vendor/glm"
IncludeDir["stb_image"] = "Eternal/vendor/stb_image"


include "Eternal/vendor/GLFW"
include "Eternal/vendor/Glad"
include "Eternal/vendor/imgui"

project "Eternal"
	location "Eternal"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ETpch.h"
	pchsource "Eternal/src/ETpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "ET_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ET_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ET_DIST"
		runtime "Release"
		optimize "on"

-------------------------------------------------------------------------------------------

project "Sandbox" 
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Eternal/src",
		"Eternal/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Eternal"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ET_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "ET_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ET_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ET_DIST"
		runtime "Release"
		optimize "on"