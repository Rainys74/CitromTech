project "nativefiledialog-extended"   -- Engine Library
        kind "StaticLib"
        language "C++"
        cppdialect (CPP_VERSION)

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")
    
        includedirs
        {
            "nativefiledialog-extended/src/include",

            "../GLFW/glfw/include"
        }

        files
        {
            "nativefiledialog-extended/src/include/**.h",
            "nativefiledialog-extended/src/include/**.hpp" -- C++ support
        }
        removefiles
        {
            "nativefiledialog-extended/src/include/nfd_sdl2.h"
        }

        links
        {
            "GLFW"
        }

        defines
        {
            "NFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE",
            
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"

            files
            {
                "nativefiledialog-extended/src/nfd_win.cpp"
            }

        filter "system:linux"
            pic "On"
    
            systemversion "latest"
            staticruntime "On"

            files
            {
                "nativefiledialog-extended/src/nfd_gtk.cpp",
                "nativefiledialog-extended/src/nfd_portal.cpp"
            }

        filter "system:macosx"
            staticruntime "On"
            systemversion "15.0"

            links
            {
                "UniformTypeIdentifiers.framework"
            }
            
            files
            {
                "nativefiledialog-extended/src/nfd_cocoa.m"
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