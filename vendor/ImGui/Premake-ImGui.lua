project "ImGui"   -- Engine Library
        kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        language "C++"
        cppdialect (CPP_VERSION)

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        --[[files 
        {
            "glfw/src/**.h",
            "glfw/src/**.c",
        }--]]
    
        includedirs
        {
            "imgui/",
            "imgui/backends/",

            "../GLFW/glfw/include" -- GLFW
        }

        files
        {
            "imgui/imconfig.h",
            "imgui/imgui.h",
            "imgui/imgui.cpp",
            "imgui/imgui_draw.cpp",
            "imgui/imgui_internal.h",
            "imgui/imgui_widgets.cpp",

            "imgui/imstb_rectpack.h",
            "imgui/imstb_textedit.h",
            "imgui/imstb_truetype.h",

            "imgui/imgui_demo.cpp",
        }

        links
        {
            "GLFW"
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"

            links 
            { 
                "gdi32", -- -lgdi32
            }
    
            files
            {
                -- Windowing
                "imgui/backends/imgui_impl_win32.cpp", -- Win32
                "imgui/backends/imgui_impl_glfw.cpp", -- GLFW

                -- OpenGL (Graphics)
                "imgui/backends/imgui_impl_opengl3.cpp", -- OpenGL
                "imgui/backends/imgui_impl_dx11.cpp" -- DirectX11
            }
    
            --[[defines 
            { 
                "_CRT_SECURE_NO_WARNINGS"
            }--]]

        filter "system:linux"
            pic "On"
    
            systemversion "latest"
            staticruntime "On"
    
            files
            {
                -- Windowing
                "imgui/backends/imgui_impl_glfw.cpp", -- GLFW

                -- Graphics
                "imgui/backends/imgui_impl_opengl3.cpp" -- OpenGL
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

            files
            {
                -- Windowing
                "imgui/backends/imgui_impl_glfw.cpp", -- GLFW

                -- Graphics
                "imgui/backends/imgui_impl_metal.mm" -- Metal
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