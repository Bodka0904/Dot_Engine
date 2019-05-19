workspace "Dot_Engine"
		architecture "x64"
		startproject "SandBox"

		configurations
		{
				"Debug",
				"Release"		
		}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Dot_Engine"
		location "Dot_Engine"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/glm/glm/**.hpp",
			"%{prj.name}/vendor/glm/glm/**.inl",
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/GLFW/include",
			"%{prj.name}/vendor/glew-2.1.0/include",
			"%{prj.name}/vendor/glm"
		}
		libdirs
		{
			"%{prj.name}/vendor/GLFW/lib-vc2017",
			"%{prj.name}/vendor/glew-2.1.0/lib/Release/x64"
		}
	
		links 
		{ 
			"glfw3",
			"glew32s",
			"opengl32"
		}
		

		filter "system:windows"
				cppdialect "C++17"
				systemversion "latest"


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
			"Dot_Engine/vendor/glm}"
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