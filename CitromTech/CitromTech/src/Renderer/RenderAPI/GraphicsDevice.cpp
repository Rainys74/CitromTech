#include "GraphicsDevice.h"

#include "CitromAssert.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DirectX11/DX11Device.h"
#endif

namespace Citrom::RenderAPI
{
    GraphicsAPI GraphicsAPIManager::s_CurrentGraphicsAPI;

    GraphicsAPI GraphicsAPIManager::GetGraphicsAPI()
    {
        return s_CurrentGraphicsAPI;
    }

    bool GraphicsAPIManager::IsGraphicsAPI(GraphicsAPI graphicsAPI)
    {
        return graphicsAPI == s_CurrentGraphicsAPI;
    }

    void GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI graphicsAPI)
    {
        s_CurrentGraphicsAPI = graphicsAPI;
    }

    bool GraphicsAPIManager::IsAPIValid(GraphicsAPI api)
    {
        switch (api)
        {
            case GraphicsAPI::DirectX11:
            {
                #ifndef CT_PLATFORM_WINDOWS
                return false;
                #else
                DX11DummyDevice dummy;
                return dummy.IsValid();
                #endif
            }
            break;
            case GraphicsAPI::OpenGL:
            {
            }
            break;
        }
        return false;
    }

    Device* Device::s_Instance = nullptr;

    Device* Device::CreateDevice()
    {
        switch (GraphicsAPIManager::GetGraphicsAPI())
        {
            case GraphicsAPI::DirectX11:
                IF_WINDOWS(return new DX11Device());
                CT_CORE_ASSERT(false, "DirectX11 is not supported on non-Windows desktops.");
                break;
            default:
                CT_CORE_ASSERT(false, "Unknown Graphics API selected!");
                return nullptr;
                break;
        }
        return nullptr;
    }
}