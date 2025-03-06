project "AngelScript"   -- Engine Library
        kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        language "C++"
        cppdialect (CPP_VERSION)

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "angelscript/source/**.cpp",
            "angelscript/source/**.h",
            "angelscript/source/**.asm",

            "angelscript/include/**.h"
        }
    
        includedirs
        {
            "angelscript/source/",
            "angelscript/include/"
        }

        filter "platforms:x64"
            removefiles { "angelscript/source/**arm**.asm", "angelscript/source/**arm**.S" }
        filter "platforms:ARM"
            removefiles { "angelscript/source/**x64**.asm", "angelscript/source/**x64**.S" }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"
    
        filter "system:linux"
            systemversion "latest"
            staticruntime "On"

        filter "system:macosx"
            staticruntime "On"
            systemversion "15.0"
    
            files
            {
                "angelscript/source/**.S"
            }
            removefiles
            {
                "angelscript/source/**gcc**.S" -- TODO: make sure this works on anything else than xcode
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