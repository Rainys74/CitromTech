#include "GraphicsDevice.h"

#include "CitromAssert.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DirectX11/DX11Device.h"
#endif

#ifdef CT_PLATFORM_MACOS
#include "GraphicsDeviceMetal.h"
#endif

#include "OpenGL/GLDevice.h"

#include <cmath> // log2
#include <algorithm> // std::max

namespace Citrom::RenderAPI
{
    static constexpr const char* GraphicsAPIToString(GraphicsAPI api)
    {
        return GraphicsAPIManager::ToString(api);
    }

    GraphicsAPI GraphicsAPIManager::s_GraphicsAPIList[static_cast<size_t>(GraphicsAPI::Count)] =
    {
        GraphicsAPI::DirectX11,
        GraphicsAPI::Metal,
        GraphicsAPI::OpenGL
    };
    GraphicsAPI& GraphicsAPIManager::s_CurrentGraphicsAPI = GraphicsAPIManager::s_GraphicsAPIList[0];

    bool GraphicsAPIManager::s_APIDecided = false;

    GraphicsAPI GraphicsAPIManager::GetGraphicsAPI()
    {
        if (!s_APIDecided)
        {
            for (size_t i = 0; i < CT_ARRAY_LENGTH(s_GraphicsAPIList); i++)
            {
                if (IsAPIValid(s_GraphicsAPIList[i]))
                {
                    s_CurrentGraphicsAPI = s_GraphicsAPIList[i];
                    s_APIDecided = true;
                    //ForceGraphicsAPI(s_GraphicsAPIList[i]);
                    break;
                }
                CT_CORE_VERBOSE("Graphics API {} at priority level ({}) is invalid, continuing.", GraphicsAPIToString(s_GraphicsAPIList[i]), i);
            }
#ifndef CT_OPTIMIZATION
            if (!s_APIDecided)
                CT_CORE_ERROR("No Valid API was found! This is undefined behaviour.");
            else
                CT_CORE_TRACE("Graphics API {} has been decided.", GraphicsAPIToString(s_CurrentGraphicsAPI));
#endif
        }

        return s_CurrentGraphicsAPI;
    }

    bool GraphicsAPIManager::IsGraphicsAPI(GraphicsAPI graphicsAPI)
    {
        CT_CORE_ASSERT_WARN(s_APIDecided, "Graphics API has not been decided yet!");
        return graphicsAPI == s_CurrentGraphicsAPI;
    }

    void GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI graphicsAPI)
    {
        CT_CORE_ASSERT_WARN(!s_APIDecided, "Graphics API is already selected!");
        s_CurrentGraphicsAPI = graphicsAPI;
        s_APIDecided = true;
        CT_CORE_TRACE("Graphics API {} has been forced.", GraphicsAPIToString(s_CurrentGraphicsAPI));
    }

    void GraphicsAPIManager::PrioritizeGraphicsAPI(GraphicsAPI graphicsAPI, uint8 priorityLevel)
    {
        CT_CORE_ASSERT_WARN(!s_APIDecided, "Graphics API is already selected!");
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
                GLDummyDevice dummy;
                return dummy.IsValid();
            }
            break;
            case GraphicsAPI::Metal:
            {
                #ifndef CT_PLATFORM_MACOS
                return false;
                #else
                return GetMetalDummyResult();
                #endif
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
            case GraphicsAPI::Metal:
                IF_MACOS(return (Device*)AllocateNewMetalDevice());
                CT_CORE_ASSERT(false, "Metal is not supported on non-Apple devices.");
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

    Format Device::FBFormatToFormat(FramebufferFormat fbFormat)
    {
#define FORMAT_CASE(x, y) case (x): return (y); break;
        switch (fbFormat)
        {
            default: return Format::Unknown; break;

            FORMAT_CASE(FramebufferFormat::None, Format::Unknown);
            FORMAT_CASE(FramebufferFormat::RGBA8, Format::R8G8B8A8_U2FNORM);
            FORMAT_CASE(FramebufferFormat::D16N, Format::Unknown);
            FORMAT_CASE(FramebufferFormat::D32F, Format::Unknown);
            FORMAT_CASE(FramebufferFormat::DEPTH24STENCIL8, Format::Unknown);
        }
    }

    uint32 Device::CalculateMipLevels(uint32 width, uint32 height)
    {
        return (uint32)log2(std::max(width, height)) + 1;
    }
}
