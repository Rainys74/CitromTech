#include "MetalDevice.h"
#include "CitromAssert.h"

namespace Citrom::RenderAPI
{
    struct PipelineStateMTL
    {
        //WRL::ComPtr<ID3D11BlendState> blendState;
        //WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
        //WRL::ComPtr<ID3D11DepthStencilState> dsState;
        
        id<MTLRenderPipelineState> pipeline;
        
        ~PipelineStateMTL()
        {
            [pipeline release];
        }
    };

    PipelineState MetalDevice::CreatePipelineState(PipelineStateDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(PipelineState, PipelineStateMTL, pipeline, internalData);
        GET_BUFFER_INTERNAL(ShaderMTL, descriptor->shader, internalShader);
        
        MTLRenderPipelineDescriptor* pd = [[MTLRenderPipelineDescriptor alloc] init];
        pd.vertexFunction = internalShader->vertexFunction;
        pd.fragmentFunction = internalShader->fragmentFunction;
        pd.colorAttachments[0].pixelFormat = m_MTLLayer.pixelFormat;
        
        NSError* error = nil;
        internalData->pipeline = [m_Device newRenderPipelineStateWithDescriptor:pd error:&error];
        CT_CORE_ASSERT(error == nil, "Failed to create a Metal Pipeline!"); // i really need a macro system for metal error checking..
        
        [pd release];

        return pipeline;
    }
    void MetalDevice::RCBindPipelineState(PipelineState* ps, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        GET_BUFFER_INTERNAL(PipelineStateMTL, ps, internalData);
        
        [internalCmd->commandEncoder setRenderPipelineState:internalData->pipeline];
    }
}
