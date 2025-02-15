#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    CommandBuffer MetalDevice::CreateCommandBuffer()
    {
        CREATE_BUFFER_INTERNAL_NO_DESC(CommandBuffer, CommandBufferMTL, cmd, internalData);
        internalData->commandBuffer = [m_CommandQueue commandBuffer];
        //internalData->commandEncoder = [internalData->commandBuffer renderCommandEncoderWithDescriptor:renderPass]; // will be created on render pass create or begin..
        
        return cmd;
    }
    void MetalDevice::BeginCommandBuffer(CommandBuffer* cmd)
    {
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalData);
        
        internalData->commandBuffer = [m_CommandQueue commandBuffer]; // recreate to overrides
        
        m_Drawable = [m_MTLLayer nextDrawable];
    }
    void MetalDevice::SubmitCommandBuffer(CommandBuffer* cmd)
    {
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalData);
        
        //[internalData->commandEncoder endEncoding];
        
        [internalData->commandBuffer presentDrawable:m_Drawable]; //[internalData->commandBuffer presentDrawable:[view currentDrawable]];
        [internalData->commandBuffer commit];
        //[internalData->commandBuffer waitUntilCompleted]; // should be in WaitForGPU() though.. Maybe cache command buffers?
    }

    CommandBuffer MetalDevice::s_RenderCommandBuffer;

    void MetalDevice::RCBegin()
    {
        BeginCommandBuffer(&s_RenderCommandBuffer);
    }
    void MetalDevice::RCEnd()
    {
        SubmitCommandBuffer(&s_RenderCommandBuffer);
        ResetCommandBuffer(&s_RenderCommandBuffer); // TODO: is this needed?
    }

    void MetalDevice::RCDraw(uint32 vertexCount, uint32 startVertexLocation, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalData);
        
        [internalData->commandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:(NSUInteger)startVertexLocation vertexCount:(NSUInteger)vertexCount];
    }
}
