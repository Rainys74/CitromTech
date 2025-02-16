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
        
        [internalData->commandBuffer release];
        internalData->commandBuffer = [m_CommandQueue commandBuffer]; // recreate to overrides
        
        m_Drawable = [m_MTLLayer nextDrawable];
    }
    void MetalDevice::SubmitCommandBuffer(CommandBuffer* cmd)
    {
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalData);
        
        //[internalData->commandEncoder endEncoding]; // in render pass
        
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

    void MetalDevice::RCDrawIndexed(uint32 indexCount, uint32 startIndex, int32 baseVertexLocation, CommandBuffer* cmd)
    {
        //glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(startIndex * sizeof(GLuint)), baseVertex);
        // or
        //glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(startIndex * sizeof(GLuint)));
        //vkCmdDrawIndexed(commandBuffer, indexCount, 1, startIndex, baseVertex, instanceOffset);
        /*
        * [encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                   indexCount:indexCount
                    indexType:MTLIndexTypeUInt32
                  indexBuffer:indexBuffer
            indexBufferOffset:startIndex * sizeof(uint32_t)
                    instanceCount:1
                   baseVertex:baseVertex];
        */
        //DXCall(m_DeviceContext->DrawIndexed(indexCount, startIndex, baseVertexLocation));
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalData);
        
        [internalData->commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                                 indexCount:indexCount
                                                  indexType:MTLIndexTypeUInt32
                                                indexBuffer:*m_CurrentIndexBuffer
                                          indexBufferOffset:startIndex * sizeof(uint32)
                                              instanceCount:1
                                                 baseVertex:baseVertexLocation
                                               baseInstance:0];
    }

    void MetalDevice::RCDraw(uint32 vertexCount, uint32 startVertexLocation, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalData);
        
        [internalData->commandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:(NSUInteger)startVertexLocation vertexCount:(NSUInteger)vertexCount];
    }
}
