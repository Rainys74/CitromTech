project "CitromTech"   -- Engine Library
        location "CitromTech"
        --kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        kind (LIBRARY_BUILD_AS_DLL and "SharedLib" or "StaticLib")
        language "C++"
        cppdialect (CPP_VERSION)
        staticruntime "on"
    
        targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/src/**.c",
        }
    
        includedirs
        {
            "%{prj.name}/src/",

            "%{IncludeDir.GLFW}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.HLSLcc}",
            
            "%{IncludeDir.glslcc}",
        }

        libdirs
        {
            --"Dependencies/**" -- ../Dependencies/glslcc_lib/Release/
        }

        links
        {
            "GLFW",
            "ImGui",
            "HLSLcc"

            --"Dependencies/GLEW 2.1.0/lib/Release/x64/glew32s.lib",
            --"opengl32.lib"
        }
        
        if LIBRARY_BUILD_AS_DLL then
            postbuildcommands
            {
                --('echo Copying %{cfg.buildtarget.relpath} to ../bin/' .. outputdir .. '/CitromEditor'),
                ("{COPY} %{cfg.buildtarget.relpath} ../../bin/" .. outputdir .. "/CitromForge"),
                --('echo Copying %{cfg.buildtarget.relpath} to ../bin/' .. outputdir .. '/CitromRuntime'),
                --("{COPY} %{cfg.buildtarget.relpath} ../../bin/" .. outputdir .. "/CitromRuntime")
            }
        end

        defines
        {
            "CT_BUILD_LIB", -- We are building the Citrom Tech Library
        }
        DefineCitromLibraryTypeMacros()

    filter { "system:linux or macosx or bsd" }
        defines 
        { 
            "CT_PLATFORM_UNIX" 
        }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest" -- 10.0 (latest installed version) or 10.0.22621.0

        -- DirectX11
        links
        {
            "d3d11.lib",
            "dxgi.lib",
            "D3DCompiler.lib"
        }

        defines
        {
            "CT_PLATFORM_WINDOWS",
            --"CT_BUILD_LIB_AS_DLL",
            --"CT_BUILD_STATIC_LIB", -- "_WINDLL" and <different options>
        }

    filter "system:linux"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CT_PLATFORM_LINUX",
            --"CT_BUILD_LIB_AS_DLL",
            --"CT_BUILD_STATIC_LIB",
        }

    filter "system:macosx"
        staticruntime "On"
        systemversion "15.0"

        defines
        {
            "CT_PLATFORM_MACOS",
            --"CT_BUILD_STATIC_LIB",
        }

    filter "configurations:Debug" -- This is NOT Windows and Debug, this is just Debug.
        defines "CT_DEBUG"
        runtime "Debug"
        symbols "On"

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

    --[[
    filter {"system:windows", "configurations:Release"}
        buildoptions "/MT"
    --]]