project "PortAudio"
        kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        language "C"
        cdialect "C99"

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")
    
    -- Source files
    files 
    {
        "portaudio/src/common/**.c", 
        "portaudio/src/common/**.h", 
        "portaudio/include/portaudio.h"
    }
    
    -- Include directories
    includedirs 
    {
        "portaudio/include",
        "portaudio/src/common"
    }

    -- Options
    filter "system:windows"
        defines
        {
            --PA_USE_SKELETON,
            --PA_USE_JACK,
            --PA_USE_ASIO,
            PA_USE_DS,
            PA_USE_WMME,
            PA_USE_WASAPI,
            PA_USE_WDMKS
        }

        defines { "_CRT_SECURE_NO_WARNINGS" }
        files 
        {
            "portaudio/src/os/win/**.c",
            "portaudio/src/os/win/**.h",
        }
        includedirs { "src/os/win" }
        links { "winmm" }

        filter { "options:PA_USE_DS" }
            files { "portaudio/src/hostapi/dsound/**.c", "portaudio/src/hostapi/dsound/**.h" }
            defines { "PA_USE_DS=1" }
            links { "dsound" }

    filter "system:linux"
        defines
        {
            --PA_USE_ALSA,
            ---PA_USE_OSS,
            --PA_USE_AUDIOIO, -- bsd maybe?
            --PA_USE_PULSEAUDIO
            --PA_USE_SNDIO
        }
        files 
        { 
            "portaudio/src/os/unix/**.c",
            "portaudio/src/os/unix/**.h"
        }
        includedirs { "portaudio/src/os/unix" }
        links { "m", "pthread" }

    filter "system:macosx"
        files 
        {
            "portaudio/src/hostapi/coreaudio/**.c",
            "portaudio/src/hostapi/coreaudio/**.h"
        }
        includedirs { "portaudio/src/hostapi/coreaudio" }
        defines { "PA_USE_COREAUDIO=1" }
        links { "CoreAudio.framework", "AudioToolbox.framework", "AudioUnit.framework", "CoreFoundation.framework", "CoreServices.framework" }

    -- Compiler options
    filter { "options:PA_WARNINGS_ARE_ERRORS", "system:windows" }
        buildoptions { "/WX", "/wd4244", "/wd4267", "/wd4996" }
    filter { "options:PA_WARNINGS_ARE_ERRORS", "system:not windows" }
        buildoptions { "-Werror", "-Wno-error=deprecated-declarations", "-Wno-error=stringop-overflow" }

    -- Configurations
    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"
    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"
    filter "configurations:Optimization"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "Full"
        symbols "Off"

-- Build options
newoption 
{
    trigger = "PA_USE_DS",
    description = "Enable support for DirectSound (Windows only)"
}
newoption 
{
    trigger = "PA_USE_COREAUDIO",
    description = "Enable support for CoreAudio (Mac only)"
}
newoption 
{
    trigger = "PA_WARNINGS_ARE_ERRORS",
    description = "Turn compiler warnings into errors"
}
newoption 
{
    trigger = "PA_BUILD_SHARED_LIBS",
    description = "Build shared library"
}
