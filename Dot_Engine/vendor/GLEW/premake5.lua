project "GLEW"
    	kind "StaticLib"
    	language "C"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
    	{
        	"include"
    	}

	files
	{
		"include/gl/*.h",	
		"src/*.c"
    	}


	
	filter "system:windows"
        	systemversion "latest"
        	staticruntime "On"
        

	defines 
	{ 
         
            	"WIN32",
      		"WIN32_LEAN_AND_MEAN",
      		"VC_EXTRALEAN",
      		"GLEW_STATIC"
	}

    	filter 
	{ 
		"system:windows", 
		"configurations:Release" 
	}
        	 
	buildoptions "/MT"
