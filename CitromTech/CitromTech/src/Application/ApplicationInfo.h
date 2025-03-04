#pragma once

#include "Platform/PlatformWindow.h"
#include "Renderer/RenderAPI/Graphics.h"

namespace Citrom
{
#define DEFAULT_TICK_TIMESTEP (1.0 / 60.0)

    struct WindowInfoSpecification
    {
        int32 width, height;
        std::string defaultTitle;

        Platform::DisplayMode displayMode;
    };
    struct GraphicsOptionsSpecification
    {
        bool test;
    };
    struct RendererInfoSpecification
    {
        RenderAPI::GraphicsAPI apiPriorityList[(size_t)RenderAPI::GraphicsAPI::Count];
    };

    struct ApplicationInfoSpecification   
    {
        WindowInfoSpecification windowInfo;
        RendererInfoSpecification rendererInfo;
        //Scene*& currentScene; // unrequired
    };

    inline ApplicationInfoSpecification MainApplicationSpec;

    class Application
    {
    public:
        void QuitGracefully()
        {
            //gameWindow->WindowShouldClose();
        }
    public:
        Platform::Window* gameWindow = nullptr;
        class LayerStack* gameLayerStack = nullptr;
        //ApplicationInfoSpecification mainApplicationSpec;

        float64 fixedTimeStep = DEFAULT_TICK_TIMESTEP; // TickTimeStep

        // Editor
        bool enableEditorUI = true; // editorUIEnabled vs enableEditorUI
    private:
    };

    extern Application* g_Application;
}