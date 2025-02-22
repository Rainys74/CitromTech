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
        td.mipmapLevelCount = 1; // TODO: hard-code for now.
        td.arrayLength = descriptor->arraySize;
        td.pixelFormat = FormatToMTLPixelFormat(descriptor->format);
        td.sampleCount = 1;
        td.usage = MTLTextureUsageShaderRead; // TODO: probably expose this
        td.storageMode = MTLStorageModeShared; // to stop the metal debugger from bitching
        
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
        
        // Sampler
        MTLSamplerDescriptor* sd = [[MTLSamplerDescriptor alloc] init];
        sd.minFilter = MTLSamplerMinMagFilterLinear;
        sd.magFilter = MTLSamplerMinMagFilterLinear;
        sd.mipFilter = MTLSamplerMipFilterLinear;
        sd.sAddressMode = MTLSamplerAddressModeRepeat; // U
        sd.tAddressMode = MTLSamplerAddressModeRepeat; // V
        sd.rAddressMode = MTLSamplerAddressModeRepeat; // W
        //sd.maxAnisotropy = descriptor->sampler.maxAnisotropy;
        //sd.lodMinClamp = descriptor->sampler.minLODClamp;
        //sd.lodMaxClamp = descriptor->sampler.maxLODClamp;
        
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
