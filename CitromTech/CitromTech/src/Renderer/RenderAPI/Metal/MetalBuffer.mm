#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    struct VertexBufferMTL
    {
        id<MTLBuffer> buffer;
        
        ~VertexBufferMTL()
        {
            [buffer release];
        }
    };

    VertexBuffer MetalDevice::CreateVertexBuffer(VertexBufferDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(VertexBuffer, VertexBufferMTL, vb, internalData);
        
        internalData->buffer = [m_Device newBufferWithBytes:descriptor->data length:descriptor->size options:MTLResourceStorageModeShared];
        //[internalData->buffer didModifyRange:<#(NSRange)#>];
        
        return vb;
    }

    void MetalDevice::RCBindVertexBuffer(VertexBuffer* vb, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        GET_BUFFER_INTERNAL(VertexBufferMTL, vb, internalData);
        
        [internalCmd->commandEncoder setVertexBuffer:internalData->buffer offset:0 atIndex:0];
    }

    struct IndexBufferMTL
    {
        id<MTLBuffer> buffer;
        
        ~IndexBufferMTL()
        {
            [buffer release];
        }
    };

    IndexBuffer MetalDevice::CreateIndexBuffer(IndexBufferDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(IndexBuffer, IndexBufferMTL, ib, internalData);
        
        internalData->buffer = [m_Device newBufferWithBytes:descriptor->data length:descriptor->count * sizeof(uint32) options:MTLResourceStorageModeShared];
        
        return ib;
    }

    void MetalDevice::RCBindIndexBuffer(IndexBuffer* ib, CommandBuffer* cmd)
    {
        //if (cmd == nullptr)
        //    cmd = &s_RenderCommandBuffer;
        //
        //GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        GET_BUFFER_INTERNAL(IndexBufferMTL, ib, internalData);
        
        m_CurrentIndexBuffer = &internalData->buffer;
    }

    struct UniformBufferMTL
    {
        id<MTLBuffer> buffer;
        
        ~UniformBufferMTL()
        {
            [buffer release];
        }
    };

    UniformBuffer MetalDevice::CreateUniformBuffer(UniformBufferDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(UniformBuffer, UniformBufferMTL, ub, internalData);
        
        internalData->buffer = [m_Device newBufferWithLength:static_cast<NSUInteger>(descriptor->dataBytes + (16 - (descriptor->dataBytes % 16))) options:MTLResourceStorageModeShared]; // NSUInteger alignedSize = (descriptor->dataBytes + 15) & ~15; // TODO: with bytes
        //[internalData->buffer didModifyRange:<#(NSRange)#>];
        
        return ub;
    }
    void MetalDevice::RCBindUniformBuffer(UniformBuffer* ub, ShaderType shaderStage, uint32 startSlot, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        GET_BUFFER_INTERNAL(UniformBufferMTL, ub, internalData);
        
        switch (shaderStage)
        {
            default:
            case ShaderType::Vertex:
                [internalCmd->commandEncoder setVertexBuffer:internalData->buffer offset:0 atIndex:startSlot];
                break;
            case ShaderType::Fragment:
                [internalCmd->commandEncoder setFragmentBuffer:internalData->buffer offset:0 atIndex:startSlot];
                break;
        }
    }
    void MetalDevice::SetUniformBufferData(UniformBuffer* ub, const void* data, const size_t size)
    {
        GET_BUFFER_INTERNAL(UniformBufferMTL, ub, internalData);

        memcpy(internalData->buffer.contents, data, size);
        //[internalData->buffer didModifyRange:<#(NSRange)#>];
    }
}
