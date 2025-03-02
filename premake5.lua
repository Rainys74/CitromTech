-- CMake Dependencies
include "Dependencies/Premake-CMake.lua"

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


IncludeDir = {}
IncludeDir["GLFW"] = "../vendor/GLFW/glfw/include"
IncludeDir["ImGui"] = "../vendor/ImGui/imgui"
IncludeDir["ImGuizmo"] = "../vendor/ImGuizmo/ImGuizmo"
IncludeDir["HLSLcc"] = "../vendor/HLSLcc/HLSLcc/include"
IncludeDir["nativefiledialog_extended"] = "../vendor/nativefiledialog-extended/nativefiledialog-extended/src/include"
IncludeDir["Lua"] = "../vendor/Lua/lua/src"
IncludeDir["AngelScript"] = "../vendor/AngelScript/angelscript/include"

-- CMake Dependencies
IncludeDir["glslcc"] = "../Dependencies/glslcc/src"


-- Configuration Settings
LIBRARY_BUILD_AS_DLL = false
RUNNER_BUILD_AS_CONSOLE_APP = true
RUNNER_APP_NAME = "CitromForge"

CPP_VERSION = "C++20" -- also known as CXX for CMake

-- Configuration Implementation Functions
function DefineCitromLibraryTypeMacros()
    if LIBRARY_BUILD_AS_DLL then
        defines
        {
            "CT_BUILD_LIB_AS_DLL" -- We are building Citrom Tech as a Dynamically Linked Library
        }
    else
        defines
        {
            "CT_BUILD_LIB_AS_STATIC" -- We are building Citrom Tech as a Static Library
        }
    end
end

include "CitromTech/Premake-CitromTech.lua"
include "CitromForge/Premake-CitromForge.lua"

-- Third Party Dependencies
include "vendor/GLFW/Premake-GLFW.lua"
include "vendor/ImGui/Premake-ImGui.lua"
include "vendor/ImGuizmo/Premake-ImGuizmo.lua"
include "vendor/HLSLcc/Premake-HLSLcc.lua"
include "vendor/nativefiledialog-extended/Premake-nativefiledialog-extended.lua"
include "vendor/Lua/Premake-Lua.lua"
include "vendor/AngelScript/Premake-AngelScript.lua"

-- Third Party Scripts
require("tools/premake/scripts/premake-vscode/vscode") --premake-vscode