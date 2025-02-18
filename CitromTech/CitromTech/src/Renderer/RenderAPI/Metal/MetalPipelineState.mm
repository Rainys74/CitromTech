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
        id<MTLDepthStencilState> dsState;
        
        ~PipelineStateMTL()
        {
            [pipeline release];
            [dsState release];
        }
    };

    PipelineState MetalDevice::CreatePipelineState(PipelineStateDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(PipelineState, PipelineStateMTL, pipeline, internalData);
        GET_BUFFER_INTERNAL(ShaderMTL, descriptor->shader, internalShader);
        GET_BUFFER_INTERNAL(VertexBufferLayoutMTL, descriptor->inputLayout, internalVBLayout);
        
        MTLRenderPipelineDescriptor* pd = [[MTLRenderPipelineDescriptor alloc] init];
        pd.vertexFunction = internalShader->vertexFunction;
        pd.fragmentFunction = internalShader->fragmentFunction;
        pd.colorAttachments[0].pixelFormat = m_MTLLayer.pixelFormat;
        
        [pd setVertexDescriptor:internalVBLayout->vd];
        
        if (descriptor->blendState)
        {
            const auto* blendDesc = descriptor->blendState;
            MTLRenderPipelineColorAttachmentDescriptor* blendState = pd.colorAttachments[0];
            
            blendState.blendingEnabled = YES;
            blendState.sourceRGBBlendFactor = BlendFactorToMTLBlendFactor(blendDesc->srcBlend);
            blendState.destinationRGBBlendFactor = BlendFactorToMTLBlendFactor(blendDesc->destBlend);
            blendState.rgbBlendOperation = BlendOpToMTLBlendOp(blendDesc->blendOperation);
            blendState.sourceAlphaBlendFactor = BlendFactorToMTLBlendFactor(blendDesc->srcBlendAlpha);
            blendState.destinationAlphaBlendFactor = BlendFactorToMTLBlendFactor(blendDesc->destBlendAlpha);
            blendState.alphaBlendOperation = BlendOpToMTLBlendOp(blendDesc->blendOperationAlpha);
            blendState.writeMask = RenderTargetWriteMaskToMTLColor(blendDesc->renderTargetWriteMask); // does this do what i expect?
        }
        //if (descriptor->rasterizerState) // In binding!
        //{
        //    const auto* rasterDesc = descriptor->rasterizerState;
        //
        //
        //    //D3D11_RASTERIZER_DESC rd = {};
        //    //rd.FillMode = FillModeToD3D11FillMode(rasterDesc->fillMode);
        //    //rd.CullMode = CullModeToD3D11CullMode(rasterDesc->cullMode);
        //    //rd.FrontCounterClockwise = rasterDesc->frontCounterClockwise;
///
        //    //DXCallHR(m_Device->CreateRasterizerState(&rd, &internalData->rasterizerState));
        //}
        // TODO: depth stencil state
        if (descriptor->dsState)
        {
            const auto* dsDesc = descriptor->dsState;
            
            MTLDepthStencilDescriptor* dsd = [[MTLDepthStencilDescriptor alloc] init];
            dsd.depthWriteEnabled = dsDesc->depthWriteEnabled;
            dsd.depthCompareFunction = MTLCompareFunctionLess;
            
            internalData->dsState = [m_Device newDepthStencilStateWithDescriptor:dsd];
            [dsd release];
            
            //D3D11_DEPTH_STENCIL_DESC dsd = {};
            //dsd.DepthEnable = dsDesc->depthEnabled;
            //dsd.DepthWriteMask = dsDesc->depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
            //dsd.DepthFunc = D3D11_COMPARISON_LESS;
            
            //DXCallHR(m_Device->CreateDepthStencilState(&dsd, &internalData->dsState));
        }
        
        m_CurrentPrimitiveType = PrimitiveTopologyToMTLType(descriptor->primitiveType);
        
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
        
        if (ps->descriptor.rasterizerState)
        {
            const RasterizerStateDesc* rasterDesc = ps->descriptor.rasterizerState;
            
            [internalCmd->commandEncoder setTriangleFillMode:FillModeToMTLTriangleFillMode(rasterDesc->fillMode)];
            [internalCmd->commandEncoder setCullMode:CullModeToMTLCullMode(rasterDesc->cullMode)];
            [internalCmd->commandEncoder setFrontFacingWinding:rasterDesc->frontCounterClockwise ? MTLWindingCounterClockwise : MTLWindingClockwise];
        }
        if (internalData->dsState && ps->descriptor.dsState && ps->descriptor.dsState->depthEnabled)
        {
            //[internalCmd->commandEncoder setDepthStencilState:internalData->dsState];
        }
        
        m_CurrentPrimitiveType = PrimitiveTopologyToMTLType(ps->descriptor.primitiveType);
        
        [internalCmd->commandEncoder setRenderPipelineState:internalData->pipeline];
    }


    MTLBlendFactor MetalDevice::BlendFactorToMTLBlendFactor(BlendFactor factor)
    {
    #define BLENDOPT_TOMTLCASE(x, y) case (x): return (y); break
        switch (factor)
        {
            default: return MTLBlendFactorOne; break;

            BLENDOPT_TOMTLCASE(BlendFactor::Zero, MTLBlendFactorZero);
            BLENDOPT_TOMTLCASE(BlendFactor::One, MTLBlendFactorOne);
            BLENDOPT_TOMTLCASE(BlendFactor::SrcAlpha, MTLBlendFactorSourceAlpha);
            BLENDOPT_TOMTLCASE(BlendFactor::OneMinusSrcAlpha, MTLBlendFactorOneMinusSourceAlpha);
        }
    }
    MTLBlendOperation MetalDevice::BlendOpToMTLBlendOp(BlendOp blendOp)
    {
        switch (blendOp)
        {
            default: return MTLBlendOperationAdd; break;

            BLENDOPT_TOMTLCASE(BlendOp::Add, MTLBlendOperationAdd);
            BLENDOPT_TOMTLCASE(BlendOp::Subtract, MTLBlendOperationSubtract);
            BLENDOPT_TOMTLCASE(BlendOp::ReverseSubtract, MTLBlendOperationReverseSubtract);
            BLENDOPT_TOMTLCASE(BlendOp::Min, MTLBlendOperationMin);
            BLENDOPT_TOMTLCASE(BlendOp::Max, MTLBlendOperationMax);
        }
    }
    MTLColorWriteMask MetalDevice::RenderTargetWriteMaskToMTLColor(RenderTargetWriteMask mask)
    {
    #define MASK_MATCH(MASK1, MASK2, DO) if (static_cast<uint8fast>(MASK1) & static_cast<uint8fast>(MASK2)) {DO;}

        MTLColorWriteMask writeMask = 0;
        MASK_MATCH(mask, RenderTargetWriteMask::Red, writeMask |= MTLColorWriteMaskRed);
        MASK_MATCH(mask, RenderTargetWriteMask::Green, writeMask |= MTLColorWriteMaskGreen);
        MASK_MATCH(mask, RenderTargetWriteMask::Blue, writeMask |= MTLColorWriteMaskBlue);
        MASK_MATCH(mask, RenderTargetWriteMask::Alpha, writeMask |= MTLColorWriteMaskAlpha);
        return writeMask;
    }

    MTLTriangleFillMode MetalDevice::FillModeToMTLTriangleFillMode(FillMode fillMode)
    {
    #define RASTOPT_TOMTLCASE(x, y) case (x): return (y); break
        switch (fillMode)
        {
            default: return MTLTriangleFillModeFill; break;

            RASTOPT_TOMTLCASE(FillMode::Solid, MTLTriangleFillModeFill);
            RASTOPT_TOMTLCASE(FillMode::Wireframe, MTLTriangleFillModeLines);
        }
    }
    MTLCullMode MetalDevice::CullModeToMTLCullMode(CullMode cullMode)
    {
        switch (cullMode)
        {
            default: return MTLCullModeBack; break;

            RASTOPT_TOMTLCASE(CullMode::Back, MTLCullModeBack);
            RASTOPT_TOMTLCASE(CullMode::Front, MTLCullModeFront);
            RASTOPT_TOMTLCASE(CullMode::None, MTLCullModeNone);
        }
    }

    MTLPrimitiveType MetalDevice::PrimitiveTopologyToMTLType(PrimitiveTopology primitives)
    {
    #define PRIMOPT_TOMTLCASE(x, y) RASTOPT_TOMTLCASE(x, y)
        switch (primitives)
        {
            default: return MTLPrimitiveTypeTriangle; break;

            PRIMOPT_TOMTLCASE(PrimitiveTopology::Triangles, MTLPrimitiveTypeTriangle);
            PRIMOPT_TOMTLCASE(PrimitiveTopology::TriangleStrips, MTLPrimitiveTypeTriangleStrip);
            PRIMOPT_TOMTLCASE(PrimitiveTopology::Points, MTLPrimitiveTypePoint);
            PRIMOPT_TOMTLCASE(PrimitiveTopology::Lines, MTLPrimitiveTypeLine);
            PRIMOPT_TOMTLCASE(PrimitiveTopology::LineStrips, MTLPrimitiveTypeLineStrip);
        }
    }
}
