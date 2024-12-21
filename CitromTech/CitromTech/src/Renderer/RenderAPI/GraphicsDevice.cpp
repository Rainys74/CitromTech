#include "GraphicsDevice.h"

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

    Device* Device::CreateDevice()
    {
        switch (GraphicsAPIManager::GetGraphicsAPI())
        {
            case GraphicsAPI::DirectX11:
                //return new DX11Device();
                break;
            default:
                CT_CORE_ASSERT(false, "Unknown Graphics API selected!");
                return nullptr;
                break;
        }
        return nullptr;
    }
}