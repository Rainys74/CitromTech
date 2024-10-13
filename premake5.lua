workspace "CitromTech" -- Solution
    --architecture "x64"
    startproject "CitromGame"

    configurations
    {
        "Debug",   -- Debug available
        "Release",  -- Faster version of debug, distribute this for engine builds.
        "Optimization" -- Aggressively optimizes all over the place, essentially use for shipping the run-time application.
    }
    platforms 
    { 
        "x64",
        "ARM" 
    }

    filter "platforms:x64"
        architecture "x64"

    filter "platforms:ARM"
        architecture "ARM"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" -- Debug-Windows-x64

-- Configuration Settings
LIBRARY_BUILD_AS_DLL = false
RUNNER_BUILD_AS_CONSOLE_APP = true

include "CitromTech/Premake-CitromTech.lua"
include "CitromForge/Premake-CitromForge.lua"

-- Third Party Dependencies
include "vendor/GLFW/Premake-GLFW.lua"

-- Third Party Scripts
require("tools/premake/scripts/premake-vscode/vscode") --premake-vscode