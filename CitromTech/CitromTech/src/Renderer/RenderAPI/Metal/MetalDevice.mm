#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    MetalDevice::MetalDevice()
    {
        m_Device = MTLCreateSystemDefaultDevice();
        m_CommandQueue = [m_Device newCommandQueue];
        
        s_RenderCommandBuffer = CreateCommandBuffer();
    }
    MetalDevice::~MetalDevice()
    {
        [m_CommandQueue release];
        [m_Device release];
    }

    /*static void queryVRAMandModelMac(uint64_t& vram, std::string& manufacturer, std::string& model)
    {
        vram = 0;
        CGError res = CGDisplayNoErr;
        // query active displays
        CGDisplayCount dspCount = 0;
        res = CGGetActiveDisplayList(0, NULL, &dspCount);
        if (res || dspCount == 0) {
            return;
        }
        // use boost here
        CGDirectDisplayID* displays = (CGDirectDisplayID*)calloc((size_t)dspCount, sizeof(CGDirectDisplayID));
        res = CGGetActiveDisplayList(dspCount, displays, &dspCount);
        if (res || dspCount == 0) {
            return;
        }
        SInt64 maxVramTotal = 0;
        for (int i = 0; i < dspCount; i++) {
            // get the service port for the display
            io_service_t dspPort = CGDisplayIOServicePort(displays[i]);
            // ask IOKit for the VRAM size property
            /* HD 2600: IOFBMemorySize = 256MB. VRAM,totalsize = 256MB
             HD 5770: IOFBMemorySize = 512MB. VRAM,totalsize = 1024MB
             Apple's QA page is not correct. We should search for IOPCIDevice's VRAM,totalsize property.
             CFTypeRef typeCode = IORegistryEntryCreateCFProperty(dspPort,
             CFSTR(kIOFBMemorySizeKey),
             kCFAllocatorDefault,
             kNilOptions);
             * /
            SInt64 vramScale = 1;
            CFTypeRef typeCode = IORegistryEntrySearchCFProperty(dspPort,
                                                                 kIOServicePlane,
                                                                 CFSTR("VRAM,totalsize"),
                                                                 kCFAllocatorDefault,
                                                                 kIORegistryIterateRecursively | kIORegistryIterateParents);
            if (!typeCode) {
                // On the new Mac Pro, we have VRAM,totalMB instead.
                typeCode = IORegistryEntrySearchCFProperty(dspPort,
                                                           kIOServicePlane,
                                                           CFSTR("VRAM,totalMB"),
                                                           kCFAllocatorDefault,
                                                           kIORegistryIterateRecursively | kIORegistryIterateParents);
                if (typeCode) {
                    vramScale = 1024 * 1024;
                }
            }
            // ensure we have valid data from IOKit
            if (typeCode) {
                SInt64 vramTotal = 0;
                if (CFGetTypeID(typeCode) == CFNumberGetTypeID()) {
                    // AMD, VRAM,totalsize is CFNumber
                    CFNumberGetValue((const __CFNumber*)typeCode, kCFNumberSInt64Type, &vramTotal);
                }
                else if (CFGetTypeID(typeCode) == CFDataGetTypeID()) {
                    // NVIDIA, VRAM,totalsize is CFData
                    CFIndex      length = CFDataGetLength((const __CFData*)typeCode);
                    const UInt8* data   = CFDataGetBytePtr((const __CFData*)typeCode);
                    if (length == 4) {
                        vramTotal = *(const unsigned int*)data;
                    }
                    else if (length == 8) {
                        vramTotal = *(const SInt64*)data;
                    }
                }
                vramTotal *= vramScale;
                CFRelease(typeCode);
                
                if (vramTotal > maxVramTotal) {
                    maxVramTotal = vramTotal;
                    typeCode = IORegistryEntrySearchCFProperty(dspPort,
                                                               kIOServicePlane,
                                                               CFSTR("NVDA,Features"),
                                                               kCFAllocatorDefault,
                                                               kIORegistryIterateRecursively | kIORegistryIterateParents);
                    if (typeCode) {
                        manufacturer = "NVIDIA";
                        CFRelease(typeCode);
                    }
                    typeCode = IORegistryEntrySearchCFProperty(dspPort,
                                                               kIOServicePlane,
                                                               CFSTR("ATY,Copyright"),
                                                               kCFAllocatorDefault,
                                                               kIORegistryIterateRecursively | kIORegistryIterateParents);
                    if (typeCode) {
                        manufacturer = "Advanced Micro Devices, Inc.";
                        CFRelease(typeCode);
                    }
                    // GPU model
                    typeCode = IORegistryEntrySearchCFProperty(dspPort,
                                                               kIOServicePlane,
                                                               CFSTR("model"),
                                                               kCFAllocatorDefault,
                                                               kIORegistryIterateRecursively | kIORegistryIterateParents);
                    if (typeCode) {
                        if (CFGetTypeID(typeCode) == CFDataGetTypeID()) {
                            model = (const char*)CFDataGetBytePtr((const __CFData*)typeCode);
                        }
                        CFRelease(typeCode);
                    }
                }
            }
        }
        vram = maxVramTotal;
    }*/

    GPUInfo MetalDevice::GetCurrentGPUInfo()
    {
        GPUInfo info;
        
        info.vendor = std::string([[m_Device name] UTF8String]); // Apple (0x106b)
        info.renderer = std::string([[m_Device name] UTF8String]);
        //info.version = std::string([[m_Device metalLibraryVersion] UTF8String]);
        info.version = "Metal Support: "; // TODO: Apple families? Mac families?
        if ([m_Device supportsFamily:MTLGPUFamilyMetal3])
            info.version += "Metal 3";
        else if ([m_Device supportsFamily:MTLGPUFamilyCommon3])
            info.version += "Common 3";
        else if ([m_Device supportsFamily:MTLGPUFamilyCommon2])
            info.version += "Common 2";
        else if ([m_Device supportsFamily:MTLGPUFamilyCommon1])
            info.version += "Common 1";
        
        //info.shadingLanguageVersion = std::string([[m_Device shadingLanguageVersion] UTF8String]);
        info.shadingLanguageVersion = "Unknown"; // TODO: there's MTLLanguageVersion i think
        info.videoMemory = [m_Device recommendedMaxWorkingSetSize]; // currentAllocatedSize for current
        
        return info;
    }
}
