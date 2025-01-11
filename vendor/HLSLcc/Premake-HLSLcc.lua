project "HLSLcc"   -- Engine Library
        kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        language "C++"
        cppdialect "C++11" --(CPP_VERSION)

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "HLSLcc/src/**.cpp",
            "HLSLcc/src/cbstring/**.c",

            "HLSLcc/include/**.h",
            "HLSLcc/include/**.hpp",
            "HLSLcc/src/**.h"
        }
    
        includedirs
        {
            "HLSLcc/include/",
            "HLSLcc/src/internal_includes/",
            "HLSLcc/src/cbstring/",
            "HLSLcc/src/",

            "HLSLcc/"
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"
    
        filter "system:linux"
            systemversion "latest"
            staticruntime "On"

        filter "system:macosx"
            staticruntime "On"
            systemversion "15.0"

            links
            {
                "Cocoa.framework",
                "IOKit.framework",
                "CoreFoundation.framework",
                "QuartzCore.framework",
            }
    
        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
    
        filter "configurations:Release"
            runtime "Release"
            optimize "on"

        filter "configurations:Optimization"
            runtime "Release"
            optimize "Full"
            symbols "Off"