#include "MetalDevice.h"

#include "imgui.h"

#include "backends/imgui_impl_metal.h"
#include "CitromAssert.h"

#import <QuartzCore/QuartzCore.h>

namespace Citrom::RenderAPI
{
	void MetalDevice::ImGuiInitGraphicsAPI()
	{
        CT_CORE_VERIFY(ImGui_ImplMetal_Init(m_Device), "Failed to Initialize ImGui Metal implementation.");
	}
    void MetalDevice::ImGuiNewFrame()
    {
        MTLRenderPassDescriptor* renderPassDescriptor = [MTLRenderPassDescriptor new];
        
        float clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};
        
        //CAMetalLayer* layer = [CAMetalLayer layer];
        //layer.device = m_Device;
        //layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        
        //layer.drawableSize = CGSizeMake(m_Width, m_Height);
        //id<CAMetalDrawable> drawable = [layer nextDrawable];
        //id<CAMetalDrawable> drawable = [m_MTKView currentDrawable];
        
        m_CommandBuffer = [m_CommandQueue commandBuffer];
        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(clear_color[0] * clear_color[3], clear_color[1] * clear_color[3], clear_color[2] * clear_color[3], clear_color[3]);
        renderPassDescriptor.colorAttachments[0].texture = m_Drawable.texture;
        renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionLoad; //MTLLoadActionClear; //or MTLLoadActionLoad
        renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        m_CommandEncoder = [m_CommandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        
        ImGui_ImplMetal_NewFrame(renderPassDescriptor);
    }
	void MetalDevice::ImGuiRenderDrawData(void* imDrawData)
	{
        ImGui_ImplMetal_RenderDrawData((ImDrawData*)imDrawData, m_CommandBuffer, m_CommandEncoder);
        
        [m_CommandEncoder endEncoding];
        [m_CommandBuffer commit];
	}
}
