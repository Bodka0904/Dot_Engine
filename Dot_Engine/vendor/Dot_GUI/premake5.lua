project "Dot_GUI"
		kind "StaticLib"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
		

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		
		}

		includedirs
		{
			"../../vendor/GLEW/include",
			"../../vendor/GLFW/include",
			"../../src",
			"../../vendor/glm"
					
		}

		filter "system:windows"
        	systemversion "latest"
        	staticruntime "On"



		filter "configurations:Debug"
				defines "D_DEBUG"
				runtime "Debug"
				symbols "on"

		filter "configurations:Release"
				defines "D_RELEASE"
				runtime "Release"
				optimize "on"
        	 
		buildoptions "/MT"