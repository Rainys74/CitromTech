#pragma once

#include "Platform/PlatformWindow.h"
#include "Renderer/RenderAPI/Graphics.h"

namespace Citrom
{
    struct WindowInfoSpecification
    {
        int32 width, height;
        const char* defaultTitle;

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
    public:
        Platform::Window* gameWindow = nullptr;
        //ApplicationInfoSpecification mainApplicationSpec;

        float64 fixedTimeStep = 1.0 / 60.0; // TickTimeStep
    private:
    };

    extern Application* g_Application;
}