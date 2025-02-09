#include "MetalDevice.h"

namespace Citrom::RenderAPI
{
    MetalDevice::MetalDevice()
    {
        m_Device = MTLCreateSystemDefaultDevice();
    }
    MetalDevice::~MetalDevice()
    {

    }

    GPUInfo MetalDevice::GetCurrentGPUInfo()
    {
        return GPUInfo();
    }
}
