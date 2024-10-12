project "GLFW"   -- Engine Library
        kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        language "C"
        cdialect "C99"

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")

        --[[files 
        {
            "glfw/src/**.h",
            "glfw/src/**.c",
        }
    
        includedirs
        {
            "glfw/include/",
        }--]]

        files
        {
            "glfw/include/GLFW/glfw3.h",
            "glfw/include/GLFW/glfw3native.h",
            
            "glfw/src/internal.h",
            "glfw/src/platform.h",
            "glfw/src/mappings.h",

            "glfw/src/context.c",
            "glfw/src/init.c",
            "glfw/src/input.c",
            "glfw/src/monitor.c",
            "glfw/src/platform.c",
            "glfw/src/vulkan.c",
            "glfw/src/window.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c",
            
            "glfw/src/null_platform.h",
            "glfw/src/null_joystick.h",

            "glfw/src/null_init.c",
            "glfw/src/null_monitor.c",
            "glfw/src/null_window.c",
            "glfw/src/null_joystick.c",
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
                -- WIN32
                "glfw/src/win32_time.h",
                "glfw/src/win32_thread.h",
                "glfw/src/win32_module.c",

                "glfw/src/win32_time.c",
                "glfw/src/win32_thread.c",

                -- WIN32
                "glfw/src/win32_platform.h",
                "glfw/src/win32_joystick.h",
                "glfw/src/win32_init.c",

                "glfw/src/win32_joystick.c",
                "glfw/src/win32_monitor.c",
                "glfw/src/win32_window.c",

                "glfw/src/wgl_context.c",
            }
    
            defines 
            { 
                "_GLFW_WIN32",
                "_CRT_SECURE_NO_WARNINGS"
            }

        filter "system:linux"
            pic "On"
    
            systemversion "latest"
            staticruntime "On"
    
            files
            {
                "glfw/src/posix_time.h",
                "glfw/src/posix_thread.h",
                "glfw/src/posix_module.c",

                "glfw/src/posix_time.c",
                "glfw/src/posix_thread.c",

                -- X11
                "glfw/src/x11_platform.h",
                "glfw/src/xkb_unicode.h",
                "glfw/src/x11_init.c",

                "glfw/src/x11_monitor.c",
                "glfw/src/x11_window.c",
                "glfw/src/xkb_unicode.c",

                "glfw/src/glx_context.c",

                -- WAYLAND
                --[[
                "glfw/src/wl_platform.h",
                "glfw/src/xkb_unicode.h",
                "glfw/src/wl_init.c",

                "glfw/src/wl_monitor.c",
                "glfw/src/wl_window.c",
                "glfw/src/xkb_unicode.c",
                --]]

                -- X11 Or WAYLAND
                -- If Linux
                "glfw/src/linux_joystick.h",
                "glfw/src/linux_joystick.c",
                -- End If Linux

                "glfw/src/posix_poll.h",
                "glfw/src/posix_poll.c",
            }
    
            defines
            {
                "_GLFW_X11",
                --"_GLFW_WAYLAND"

                -- Workaround for -std=c99 on Linux disabling _DEFAULT_SOURCE (POSIX 2008 and more)
                "_DEFAULT_SOURCE",
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
                -- APPLE
                "glfw/src/cocoa_time.h",
                "glfw/src/cocoa_time.c",
                "glfw/src/posix_thread.h",

                "glfw/src/posix_module.c",
                "glfw/src/posix_thread.c",

                -- COCOA
                "glfw/src/cocoa_platform.h",
                "glfw/src/cocoa_joystick.h",
                "glfw/src/cocoa_init.m",

                "glfw/src/cocoa_joystick.m",
                "glfw/src/cocoa_monitor.m",
                "glfw/src/cocoa_window.m",

                "glfw/src/nsgl_context.m",
            }
    
            defines
            {
                "_GLFW_COCOA"
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