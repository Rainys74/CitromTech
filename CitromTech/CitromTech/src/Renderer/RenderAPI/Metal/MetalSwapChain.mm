#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    void MetalDevice::RCSetViewport(const ViewportSpecification& viewport, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        
        // Configure viewport
        MTLViewport vp;
        vp.width = viewport.width * (float64)m_Width;
        vp.height = viewport.height * (float64)m_Height; // TODO: goes from top to bottom but i believe it should be the opposite..
        vp.znear = viewport.zNear;
        vp.zfar = viewport.zFar;
        vp.originX = viewport.xPos * (float64)m_Width; // xPos
        vp.originY = -viewport.yPos * (float64)m_Height; // yPos
        
        [internalCmd->commandEncoder setViewport:vp];
    }

    MTLPixelFormat MetalDevice::FormatToMTLPixelFormat(Format format)
    {
    #define FORMAT_TOMTLPIXELCASE(x, y) case (x): return (y); break
        switch (format)
        {
            default: return MTLPixelFormatBGRA8Unorm; break;

            FORMAT_TOMTLPIXELCASE(Format::Unknown, MTLPixelFormatBGRA8Unorm);
            FORMAT_TOMTLPIXELCASE(Format::R32_FLOAT, MTLPixelFormatR32Float);
            FORMAT_TOMTLPIXELCASE(Format::R32G32_FLOAT, MTLPixelFormatRG32Float);
            FORMAT_TOMTLPIXELCASE(Format::R32G32B32A32_FLOAT, MTLPixelFormatRGBA32Float);

            FORMAT_TOMTLPIXELCASE(Format::R8_U2FNORM, MTLPixelFormatR8Unorm);
            FORMAT_TOMTLPIXELCASE(Format::R8G8_U2FNORM, MTLPixelFormatRG8Unorm);
            FORMAT_TOMTLPIXELCASE(Format::R8G8B8A8_U2FNORM, MTLPixelFormatRGBA8Unorm);
            FORMAT_TOMTLPIXELCASE(Format::B8G8R8A8_U2FNORM, MTLPixelFormatBGRA8Unorm);
        }
    }
}
