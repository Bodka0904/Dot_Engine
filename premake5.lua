workspace "Dot_Engine"
		architecture "x64"
		startproject "SandBox"

		configurations
		{
				"Debug",
				"Release"		
		}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Dot_Engine/vendor/GLFW/include"
IncludeDir["GLEW"] = "Dot_Engine/vendor/GLEW/include"
IncludeDir["Dot_GUI"] = "Dot_Engine/vendor/Dot_GUI"


include "Dot_Engine/vendor/GLFW"
include "Dot_Engine/vendor/GLEW"
include "Dot_Engine/vendor/Dot_GUI"


project "Dot_Engine"
		location "Dot_Engine"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
		
		pchheader "stdafx.h"
		pchsource "Dot_Engine/src/stdafx.cpp"
		
		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/glm/glm/**.hpp",
			"%{prj.name}/vendor/glm/glm/**.inl",
			"%{prj.name}/vendor/stb_image/**.h",
			"%{prj.name}/vendor/stb_image/**.cpp"
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"GLEW_STATIC"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{IncludeDir.GLFW}",	
			"%{IncludeDir.GLEW}",
			"%{IncludeDir.Dot_GUI}",
			"%{prj.name}/vendor/glm",
			"%{prj.name}/vendor/stb_image"
		}


		links 
		{ 
			"GLEW",
			"GLFW",
			"Dot_GUI",
			"opengl32"
		}
		

		filter "system:windows"
				cppdialect "C++17"
				systemversion "latest"
			defines
			{
				"GLFW_INCLUDE_NONE",
				"GLEW_STATIC",
				"D_PLATFORM_WINDOWS"
			}
			


		filter "configurations:Debug"
				defines "D_DEBUG"
				runtime "Debug"
				symbols "on"

		filter "configurations:Release"
				defines "D_RELEASE"
				runtime "Release"
				optimize "on"

project "SandBox"
		location "SandBox"
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
			"Dot_Engine/vendor/GLFW/include",
			"Dot_Engine/vendor/GLEW/include",
			"Dot_Engine/vendor/Dot_GUI",
			"Dot_Engine/vendor",
			"Dot_Engine/src",
			"Dot_Engine/vendor/glm"
		}
		

		links
		{
			"Dot_Engine"
		}

		filter "system:windows"
				systemversion "latest"

		filter "configurations:Debug"
				defines "D_DEBUG"
				runtime "Debug"
				symbols "on"

		filter "configurations:Release"
				defines "D_RELEASE"
				runtime "Release"
				optimize "on"