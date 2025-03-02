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
        RenderAPI::VSyncMode defaultVSync;
    };

    struct ApplicationInfoSpecification   
    {
        WindowInfoSpecification windowInfo;
        RendererInfoSpecification rendererInfo;

        Platform::Window* mainWindow;
        //Scene*& currentScene;
    };

    inline ApplicationInfoSpecification MainApplicationSpec;
}