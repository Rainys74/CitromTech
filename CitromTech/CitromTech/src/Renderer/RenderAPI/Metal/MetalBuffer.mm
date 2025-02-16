#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
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
        
        internalData->buffer = [m_Device newBufferWithLength:static_cast<NSUInteger>(descriptor->dataBytes + (16 - (descriptor->dataBytes % 16))) options:MTLResourceStorageModeShared]; // NSUInteger alignedSize = (descriptor->dataBytes + 15) & ~15;
        
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
    }
}
