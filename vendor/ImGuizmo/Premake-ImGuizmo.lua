project "ImGuizmo"   -- Engine Library
        kind "StaticLib" -- choose between StaticLib (lib) and SharedLib (dll)
        language "C++"
        cppdialect (CPP_VERSION)

        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-intermediate/" .. outputdir .. "/%{prj.name}")
    
        includedirs
        {
            "ImGuizmo/",

            "../ImGui/imgui/"
        }

        files
        {
            --"ImGuizmo/**.cpp",
            --"ImGuizmo/**.h",

            "ImGuizmo/ImGuizmo.h",
            "ImGuizmo/ImGuizmo.cpp"
        }

        links
        {
            "ImGui"
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"

        filter "system:linux"
            pic "On"
    
            systemversion "latest"
            staticruntime "On"

        filter "system:macosx"
            staticruntime "On"
            systemversion "15.0"
            
    
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