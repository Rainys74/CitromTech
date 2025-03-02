project "Lua"   -- Engine Library
        kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        language "C"
        cdialect "C99"

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "lua/src/**.c",
            "lua/src/**.h"
        }
    
        includedirs
        {
            "lua/src/"
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