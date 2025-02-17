#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    struct VertexBufferMTL
    {
        id<MTLBuffer> buffer;
        
        ~VertexBufferMTL()
        {
            if (METAL->m_CurrentVertexBuffer == &buffer)
                METAL->m_CurrentVertexBuffer = nullptr;
            
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
        
        m_CurrentVertexBuffer = &internalData->buffer;
    }
    VertexBufferLayout MetalDevice::CreateVertexBufferLayout(VertexBufferLayoutDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL_NO_DESC(VertexBufferLayout, VertexBufferLayoutMTL, vbLayout, internalData); // should probably be called an Input Layout, or not.
        
        //// input (vertex) layout
        //CTL::DArray<D3D11_INPUT_ELEMENT_DESC> ieds;
        //UINT offset = 0; // also known as pointer in OpenGL
        //for (auto& element : descriptor->layoutElements)
        //{
        //    D3D11_INPUT_ELEMENT_DESC ied = {};
        //    ied.SemanticName = element.elementName.c_str();
        //    ied.SemanticIndex = element.elementID;
        //    ied.Format = FormatToDXGIFormat(element.elementFormat);
        //    ied.InputSlot = 0; // not important
        //    ied.AlignedByteOffset = offset; // offset between the first item and the current in bytes
//
        //    offset += GetFormatSize(element.elementFormat);
//
        //    // Instancing stuff (not important)
        //    ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        //    ied.InstanceDataStepRate = 0;
//
        //    ieds.PushBack(ied);
        //}
        
        const auto* attributes = [internalData->vd attributes];
        
        // input (vertex) layout
        uint32 i = 0, offset = 0; // also known as pointer in OpenGL
        for (auto& element : descriptor->layoutElements)
        {
            const auto* ied = attributes[i];
            
            [ied setBufferIndex:0]; //i]; //element.elementID];
            [ied setFormat:FormatToMTLVertexFormat(element.elementFormat)];
            [ied setOffset:offset]; // offset between the first item and the current in bytes
            
            //ied.SemanticName = element.elementName.c_str();
            //ied.SemanticIndex = element.elementID;
            //ied.Format = FormatToDXGIFormat(element.elementFormat);
            //ied.InputSlot = 0; // not important
            //ied.AlignedByteOffset = offset; // offset between the first item and the current in bytes

            offset += GetFormatSize(element.elementFormat);
            i++;
        }
        
        // TODO: is this correct?
        const auto* layoutDescriptor = [internalData->vd layouts][0];
        [layoutDescriptor setStride:offset];
        //[layoutDescriptor setStepRate:0];
        
        //for (auto& element : descriptor->layoutElements)
        /*for (int j = 0; j < i; j++)
        {
            const auto* layoutDescriptor = [internalData->vd layouts][j];//[element.elementID];
            [layoutDescriptor setStride:offset];
            //[layoutDescriptor setStepRate:0];
        }*/
        
        // input (vertex) layout
        /*NSUInteger i = 0, offset = 0; // also known as pointer in OpenGL
        for (auto& element : descriptor->layoutElements)
        {
            MTLVertexAttributeDescriptor* vad = [MTLVertexAttributeDescriptor new];
            
            vad.bufferIndex = element.elementID;
            vad.format = FormatToMTLVertexFormat(element.elementFormat);
            vad.offset = offset;
            
            //ied.SemanticName = element.elementName.c_str();
            //ied.SemanticIndex = element.elementID;
            //ied.Format = FormatToDXGIFormat(element.elementFormat);
            //ied.InputSlot = 0; // not important
            //ied.AlignedByteOffset = offset; // offset between the first item and the current in bytes

            offset += GetFormatSize(element.elementFormat);
            i++;
        }*/
        
        return vbLayout;
    }

    MTLVertexFormat MetalDevice::FormatToMTLVertexFormat(Format format)
    {
    #define FORMAT_TOMTLVERTEXCASE(x, y) case (x): return (y); break
        switch (format)
        {
            default: return MTLVertexFormatFloat3; break;

            FORMAT_TOMTLVERTEXCASE(Format::Unknown, MTLVertexFormatFloat3);
            FORMAT_TOMTLVERTEXCASE(Format::R32_FLOAT, MTLVertexFormatFloat);
            FORMAT_TOMTLVERTEXCASE(Format::R32G32_FLOAT, MTLVertexFormatFloat2);
            FORMAT_TOMTLVERTEXCASE(Format::R32G32B32_FLOAT, MTLVertexFormatFloat3);
            FORMAT_TOMTLVERTEXCASE(Format::R32G32B32A32_FLOAT, MTLVertexFormatFloat4);

            FORMAT_TOMTLVERTEXCASE(Format::R8_U2FNORM, MTLVertexFormatUCharNormalized);
            FORMAT_TOMTLVERTEXCASE(Format::R8G8_U2FNORM, MTLVertexFormatUChar2Normalized);
            FORMAT_TOMTLVERTEXCASE(Format::R8G8B8A8_U2FNORM, MTLVertexFormatUChar3Normalized);
            FORMAT_TOMTLVERTEXCASE(Format::B8G8R8A8_U2FNORM, MTLVertexFormatUChar4Normalized);
        }
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
#define GPU_BYTE_ALIGNMENT (16)
#define GPU_ALLIGNED_SIZE(x) ((x) + (GPU_BYTE_ALIGNMENT - ((x) % GPU_BYTE_ALIGNMENT)))

    UniformBuffer MetalDevice::CreateUniformBuffer(UniformBufferDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(UniformBuffer, UniformBufferMTL, ub, internalData);
        
        internalData->buffer = [m_Device newBufferWithLength:static_cast<NSUInteger>(GPU_ALLIGNED_SIZE(descriptor->dataBytes)) options:MTLResourceStorageModeManaged]; // NSUInteger alignedSize = (descriptor->dataBytes + 15) & ~15; // TODO: with bytes // MTLResourceStorageModeShared
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
                [internalCmd->commandEncoder setVertexBuffer:internalData->buffer
                                                      offset:0
                                                     atIndex: (m_CurrentVertexBuffer == nullptr) ? startSlot : startSlot + 1];
                break;
            case ShaderType::Fragment:
                [internalCmd->commandEncoder setFragmentBuffer:internalData->buffer offset:0 atIndex:startSlot];
                break;
        }
    }
    void MetalDevice::SetUniformBufferData(UniformBuffer* ub, const void* data, const size_t size)
    {
        GET_BUFFER_INTERNAL(UniformBufferMTL, ub, internalData);

        memcpy(internalData->buffer.contents, data, GPU_ALLIGNED_SIZE(size));
        [internalData->buffer didModifyRange:NSMakeRange(0, GPU_ALLIGNED_SIZE(size))];
    }

    void MetalDevice::SetName(VertexBuffer* resource, const char* name)
    {
        GET_BUFFER_INTERNAL(VertexBufferMTL, resource, internalData);
        TSetResourceNameMTL(internalData->buffer, name);
    }
    //void MetalDevice::SetName(VertexBufferLayout* resource, const char* name)
    //{
    //    GET_BUFFER_INTERNAL(VertexBufferLayoutMTL, resource, internalData);
    //    TSetResourceNameMTL(internalData->vd, name);
    //}
    void MetalDevice::SetName(IndexBuffer* resource, const char* name)
    {
        GET_BUFFER_INTERNAL(IndexBufferMTL, resource, internalData);
        TSetResourceNameMTL(internalData->buffer, name);
    }
    void MetalDevice::SetName(UniformBuffer* resource, const char* name)
    {
        GET_BUFFER_INTERNAL(UniformBufferMTL, resource, internalData);
        TSetResourceNameMTL(internalData->buffer, name);
    }
}
