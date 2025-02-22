#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    struct Texture2DMTL
    {
        id<MTLTexture> texture;
        id<MTLSamplerState> sampler;
        
        ~Texture2DMTL()
        {
            [texture release];
            [sampler release];
        }
    };

    Texture2D MetalDevice::CreateTexture2D(Texture2DDesc* descriptor)
    {
        CREATE_BUFFER_INTERNAL(Texture2D, Texture2DMTL, tex2D, internalData);
        
        MTLTextureDescriptor* td = [MTLTextureDescriptor new];
        td.width = descriptor->width;
        td.height = descriptor->height;
        td.mipmapLevelCount = (descriptor->mipLevels == MIP_LEVELS_MAX) ? CalculateMipLevels(descriptor->width, descriptor->height) : descriptor->mipLevels;
        td.arrayLength = descriptor->arraySize;
        td.pixelFormat = FormatToMTLPixelFormat(descriptor->format);
        td.sampleCount = 1;
        td.usage = 0;
        td.storageMode = MTLStorageModeShared; // to stop the metal debugger from bitching
        
        if (HAS_FLAG(descriptor->flags, TextureFlags::ShaderReadAccess))    td.usage |= MTLTextureUsageShaderRead;
        if (HAS_FLAG(descriptor->flags, TextureFlags::ShaderWriteAccess))   td.usage |= MTLTextureUsageShaderWrite;
        
        if (descriptor->mipLevels != MIP_LEVELS_NONE)
            td.usage |= MTLTextureUsageRenderTarget;
        
        if (descriptor->arraySize > 1)
            td.textureType = (0 > 1) ? MTLTextureType2DMultisampleArray : MTLTextureType2DArray; // TODO: replace 0 with samples
        else
            td.textureType = (0 > 1) ? MTLTextureType2DMultisample : MTLTextureType2D;
        
        //td.MipLevels = descriptor->mipLevels;
        //td.ArraySize = descriptor->arraySize;
        //td.Format = FormatToDXGIFormat(descriptor->format);
        //td.SampleDesc.Count = 1;
        //td.SampleDesc.Quality = 0;
        //td.Usage = UsageToD3D11Usage(descriptor->usage);
        //td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        //td.CPUAccessFlags = 0;
        
        internalData->texture = [m_Device newTextureWithDescriptor:td];
        
        MTLRegion region = MTLRegionMake2D(0, 0, descriptor->width, descriptor->height);
        [internalData->texture replaceRegion:region
                                 mipmapLevel:0
                                   withBytes:descriptor->data
                                 bytesPerRow:static_cast<NSUInteger>(descriptor->width * GetFormatSize(descriptor->format))];
        
        [td release];
        
        // Mip-Maps
        if (descriptor->mipLevels != MIP_LEVELS_NONE)
        {
            id<MTLCommandBuffer> commandBuffer = [m_CommandQueue commandBuffer]; // don't know if it's a good idea creating another command buffer here.
            
            id<MTLBlitCommandEncoder> encoder = [commandBuffer blitCommandEncoder];
            [encoder generateMipmapsForTexture: internalData->texture];
            [encoder endEncoding];
            
            [commandBuffer commit];
        }
            
        // Sampler
        MTLSamplerDescriptor* sd = [[MTLSamplerDescriptor alloc] init];
        sd.minFilter = MTLSamplerMinMagFilterLinear;
        sd.magFilter = MTLSamplerMinMagFilterLinear; // Bilinear filtering?
        sd.mipFilter = MTLSamplerMipFilterLinear; // Trilinear filtering? (required for mip-maps?)
        sd.sAddressMode = MTLSamplerAddressModeRepeat; // U
        sd.tAddressMode = MTLSamplerAddressModeRepeat; // V
        sd.rAddressMode = MTLSamplerAddressModeRepeat; // W
        sd.maxAnisotropy = (descriptor->sampler.maxAnisotropy == MAX_ANISOTROPY) ? 16 : descriptor->sampler.maxAnisotropy;
        sd.lodMinClamp = descriptor->sampler.minLODClamp;
        sd.lodMaxClamp = descriptor->sampler.maxLODClamp;
        
        internalData->sampler = [m_Device newSamplerStateWithDescriptor:sd];
        
        [sd release];
        
        return tex2D;
    }
    void MetalDevice::RCBindTexture2D(Texture2D* tex2D, uint32 startSlot, CommandBuffer* cmd)
    {
        if (cmd == nullptr)
            cmd = &s_RenderCommandBuffer;
        
        GET_BUFFER_INTERNAL(CommandBufferMTL, cmd, internalCmd);
        GET_BUFFER_INTERNAL(Texture2DMTL, tex2D, internalData);
        
        // Mip-Maps
        //if (!internalData->mipMapsGenerated && tex2D->descriptor.mipLevels != MIP_LEVELS_NONE)
        //{
        //    id<MTLBlitCommandEncoder> encoder = [internalCmd->commandBuffer blitCommandEncoder];
        //    [encoder generateMipmapsForTexture: internalData->texture];
        //    [encoder endEncoding];
        //
        //    internalData->mipMapsGenerated = true;
        //}
        
        // TODO: this shit
        if (tex2D->descriptor.arraySize > 1)
        {
            NSRange range;
            //[internalCmd->commandEncoder setFragmentTextures:internalData->texture withRange:range];
        }
        else
        {
            [internalCmd->commandEncoder setFragmentTexture:internalData->texture atIndex:startSlot];
            [internalCmd->commandEncoder setFragmentSamplerState:internalData->sampler atIndex:startSlot];
        }
    }
}
