#include "GraphicsDeviceMetal.h"

#ifdef CT_PLATFORM_MACOS
#include "Metal/MetalDevice.h"
#endif

namespace Citrom::RenderAPI
{
	bool GetMetalDummyResult()
    {
        MetalDummyDevice dummy;
        return dummy.IsValid();
    }
    void* AllocateNewMetalDevice()
    {
        return new MetalDevice();
    }
}
