#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    struct FramebufferMTL
    {
        bool test;
    };

    Framebuffer MetalDevice::CreateFramebuffer(FramebufferDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(Framebuffer, FramebufferMTL, fb, internalData);
        
        return fb;
    }

    RenderPass MetalDevice::CreateRenderPass(RenderPassDesc* descriptor)
    {
        RenderPass pass = {};
        pass.descriptor = *descriptor;
        return pass;
    }
    void MetalDevice::RCBeginRenderPass(RenderPass* pass, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        
        const auto* fbDesc = &pass->descriptor.targetFramebuffer->descriptor;
        
        MTLRenderPassDescriptor* rpd = [MTLRenderPassDescriptor renderPassDescriptor]; // view->currentRendersPassDescriptor() if fb nullptr
        if (pass->descriptor.targetFramebuffer == nullptr)
        {
            rpd.colorAttachments[0].texture = m_Drawable.texture;
            rpd.colorAttachments[0].clearColor = MTLClearColorMake(0.45, 0.55, 0.60, 1.0); // Clear color
            rpd.colorAttachments[0].loadAction = MTLLoadActionClear; // Clear the texture
            rpd.colorAttachments[0].storeAction = MTLStoreActionStore; // Store the texture for display
        }
        else
        {
            GET_BUFFER_INTERNAL(FramebufferMTL, pass->descriptor.targetFramebuffer, internalFB);
        }
        
        //internalCmd->commandEncoder = [internalCmd->commandBuffer renderCommandEncoderWithDescriptor:rpd]; // TODO: uncomment these 2 when ready
    }
    void MetalDevice::RCEndRenderPass(CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        
        //[internalCmd->commandEncoder endEncoding];
    }
}
