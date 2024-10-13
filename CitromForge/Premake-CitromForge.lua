project "CitromForge" -- EXE
        location "CitromForge"
        --kind "ConsoleApp" -- ConsoleApp or WindowedApp
        kind (RUNNER_BUILD_AS_CONSOLE_APP and "ConsoleApp" or "WindowedApp")
        language "C++"
        cppdialect "C++17"

        targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs
        {
            "../CitromTech/CitromTech/src"
        }

        links
        {
            "CitromTech",
            
            "GLFW",
        }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CT_PLATFORM_WINDOWS",
        }

    filter "system:linux"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CT_PLATFORM_LINUX",
        }

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

        defines
        {
            "CT_PLATFORM_MACOS",
        }

    filter "configurations:Debug"
        defines "CT_DEBUG"
        runtime "Debug"
        symbols "On"
        --[[--system ( "windows", { "console" } ) -- Set subsystem to console for debugging only for Windows

        prebuildcommands 
        {
            "/SUBSYSTEM:CONSOLE"
        }--]]

    filter "configurations:Release"
        defines "CT_RELEASE"
        runtime "Release"
        optimize "On"
        symbols "Off"

    filter "configurations:Optimization"
        defines "CT_OPTIMIZATION"
        runtime "Release"
        optimize "Full"
        symbols "Off"