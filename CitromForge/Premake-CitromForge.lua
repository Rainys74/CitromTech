project "CitromForge" -- EXE
        location "CitromForge"
        --kind "ConsoleApp" -- ConsoleApp or WindowedApp
        kind (RUNNER_BUILD_AS_CONSOLE_APP and "ConsoleApp" or "WindowedApp")
        language "C++"
        cppdialect (CPP_VERSION)

        targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        DEBUG_WORKING_DIRECTORY = "../WorkingDirectory"
        debugdir (DEBUG_WORKING_DIRECTORY)

        targetname (RUNNER_APP_NAME)

        files 
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs
        {
            "%{prj.name}/src/",

            "../CitromTech/CitromTech/src",

            "%{IncludeDir.ImGui}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.nativefiledialog_extended}",
            "%{IncludeDir.Lua}"
        }

        defines
        {
            "CT_APP_NAME="..(RUNNER_APP_NAME),
        }

        if RUNNER_BUILD_AS_CONSOLE_APP then
            defines
            {
                "CT_BUILD_APP_AS_CONSOLE" -- Check to see if CitromForge is being built as a console application
            }
        end

        links
        {
            "CitromTech",
            
            "GLFW",
            "ImGui",
            "ImGuizmo",
            "HLSLcc",
            "nativefiledialog-extended",
            "Lua"
        }
        DefineCitromLibraryTypeMacros()

        postbuildcommands
        {
            --('echo Copying %{cfg.buildtarget.relpath} to ../bin/' .. outputdir .. '/CitromEditor'),
            -- Uses the path from the project file (.vcxproj, Makefile, .xcodeproj) maybe %{wks.location} can be used?
            ("{MKDIR} ../../WorkingDirectory"),
            ("{COPY} %{cfg.buildtarget.abspath} ../../WorkingDirectory"),
            -- TODO: On debug or release/optimization builds copy over Debug or Release CMake generated dlls to the working dir
            ("{COPY} ../../Dependencies/glslcc_dll/Debug/glslcc.dll ../../WorkingDirectory"), --%{cfg.buildtarget.abspath}
        }

    filter { "system:linux or macosx or bsd" }
        defines 
        { 
            "CT_PLATFORM_UNIX"
        }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CT_PLATFORM_WINDOWS",
        }

        files
        {
            "../Resources/resources.rc",
            "../Resources/**.ico"
        }
        --vpaths { ["Resources/*"] = { "*.rc", "**.ico" } }

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
            "UniformTypeIdentifiers.framework",
            "Metal.framework"
        }

        defines
        {
            "CT_PLATFORM_MACOS",
        }
        
        xcodebuildsettings 
        {
            ["GENERATE_INFOPLIST_FILE"] = "YES", -- Enable automatic generation of Info.plist
            ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.citromstudio.citromforge",
            ["WORKING_DIRECTORY"] = DEBUG_WORKING_DIRECTORY
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