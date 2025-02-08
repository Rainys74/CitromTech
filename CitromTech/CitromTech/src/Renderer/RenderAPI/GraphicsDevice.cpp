#include "GraphicsDevice.h"

#include "CitromAssert.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DirectX11/DX11Device.h"
#endif

#include "OpenGL/GLDevice.h"

namespace Citrom::RenderAPI
{
    GraphicsAPI GraphicsAPIManager::s_GraphicsAPIList[static_cast<size_t>(GraphicsAPI::Count)] =
    {
        GraphicsAPI::DirectX11,
        GraphicsAPI::OpenGL
    };
    GraphicsAPI& GraphicsAPIManager::s_CurrentGraphicsAPI = GraphicsAPIManager::s_GraphicsAPIList[0];

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

    void GraphicsAPIManager::PrioritizeGraphicsAPI(GraphicsAPI graphicsAPI, uint8 priorityLevel)
    {
        s_GraphicsAPIList[priorityLevel] = graphicsAPI;
    }

    GraphicsAPI GraphicsAPIManager::GetGraphicsAPIAtPriority(uint8 priorityLevel)
    {
        return s_GraphicsAPIList[priorityLevel];
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
                //GLDummyDevice dummy;
                //return dummy.IsValid();
                return true;
            }
            break;
            default:
            {
                CT_CORE_ERROR("Unknown API passed into IsAPIValid()!");
                return false;
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
            case GraphicsAPI::OpenGL:
                return new GLDevice();
                break;
            default:
                CT_CORE_ASSERT(false, "Unknown Graphics API selected!");
                return nullptr;
                break;
        }
        return nullptr;
    }

    size_t Device::GetFormatSize(Format format)
    {
#define FORMATSIZE_CASE(x, y, z) case (x): return sizeof(y) * (z); break

        switch (format)
        {
            default: case Format::Unknown: return 0; break;

            FORMATSIZE_CASE(Format::R32_FLOAT, float32, 1);
            FORMATSIZE_CASE(Format::R32G32_FLOAT, float32, 2);
            FORMATSIZE_CASE(Format::R32G32B32_FLOAT, float32, 3);
            FORMATSIZE_CASE(Format::R32G32B32A32_FLOAT, float32, 4);

            FORMATSIZE_CASE(Format::R8G8B8A8_U2FNORM, uint8, 4); // TODO: CONTINUE
        }
        return 0;
    }
    size_t Device::GetFBFormatSize(FramebufferFormat format)
    {
#define FORMATSIZE_CASE_MANUAL(x, y) case (x): return (y); break
        switch (format)
        {
            default: case FramebufferFormat::None: return 0; break;

            FORMATSIZE_CASE(FramebufferFormat::RGBA8, uint8, 4);
            FORMATSIZE_CASE(FramebufferFormat::D32F, float32, 1);
            FORMATSIZE_CASE_MANUAL(FramebufferFormat::DEPTH24STENCIL8, 4 /*32 / 8*/);
        }
        return 0;
    }

    size_t Device::GetLayoutStride(const VertexBufferLayoutDesc* vbLayoutSpec)
    {
        size_t stridePerVertex = 0;
        for (auto& element : vbLayoutSpec->layoutElements)
        {
            stridePerVertex += GetFormatSize(element.elementFormat);
        }
        return stridePerVertex;
    }
}