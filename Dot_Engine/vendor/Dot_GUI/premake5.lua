project "Dot_GUI"
		kind "StaticLib"
		language "C++"
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
			"%{prj.name}/src",
			"%{prj.name}/res",
			"%{prj.name}/vendor/GLEW/include",
			"%{prj.name}/vendor/GLFW/include",
			"%{prj.name}/vendor/glm",
			"%{prj.name}/vendor/stb_image"
					
		}

		filter "system:windows"
        	systemversion "latest"
        	staticruntime "On"

		filter "configurations:Debug"
				runtime "Debug"
				symbols "on"

		filter "configurations:Release"
				runtime "Release"
				optimize "on"
        	 
	