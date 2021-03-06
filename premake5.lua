workspace "Eternal"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Doomed_Editor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (in our case solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Eternal/vendor/GLFW/include"
IncludeDir["Glad"] = "Eternal/vendor/Glad/include"
IncludeDir["ImGui"] = "Eternal/vendor/imgui"
IncludeDir["glm"] = "Eternal/vendor/glm"
IncludeDir["stb_image"] = "Eternal/vendor/stb_image"
IncludeDir["entt"] = "Eternal/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Eternal/vendor/yaml-cpp/include"
IncludeDir["box2d"] = "Eternal/vendor/box2d/include"
IncludeDir["HazelAudio"] = "Eternal/vendor/Audio/HazelAudio/src"

group "Dependencies"
	include "Eternal/vendor/GLFW"
	include "Eternal/vendor/Glad"
	include "Eternal/vendor/imgui"
	include "Eternal/vendor/yaml-cpp"
	include "Eternal/vendor/box2d"
group ""

group "Dependencies/Audio"
	include "Eternal/vendor/Audio"
group ""

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
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/assets/scripts/**.h"
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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.HazelAudio}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"box2d",
		"HazelAudio",
		"opengl32.lib",
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

project "ProjectQuack" 
	location "ProjectQuack"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.HazelAudio}"
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

-------------------------------------------------------------------------------------------

project "Doomed_Editor" 
	location "Doomed_Editor"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.HazelAudio}"
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