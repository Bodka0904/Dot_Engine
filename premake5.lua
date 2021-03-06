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

include "Dot_Engine/vendor/GLFW"
include "Dot_Engine/vendor/GLEW"


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
			"%{prj.name}/res",
			"%{IncludeDir.GLFW}",	
			"%{IncludeDir.GLEW}",
			"%{prj.name}/vendor/glm",
			"%{prj.name}/vendor/stb_image",
			"%{prj.name}/vendor/assimp/include"
			
		}
	

		links 
		{ 
			"GLEW",
			"GLFW",
			"opengl32"	
		}
		

		filter "system:windows"
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
		
			"Dot_Engine/vendor",
			"Dot_Engine/src",
			"Dot_Engine/vendor/glm",
			"Dot_Engine/vendor/assimp/include",
			"Dot_Engine/vendor/Dot_Gui"
		}
		
		
		links
		{
			"Dot_Engine",
			"Dot_Engine/vendor/assimp/win64/assimp.lib"			
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