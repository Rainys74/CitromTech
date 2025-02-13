#include "GLDevice.h"

namespace Citrom::RenderAPI
{
    GLDevice::GLDevice()
    {

    }
    GLDevice::~GLDevice()
    {

    }

    void GLDevice::WaitForGPU()
    {
        // TODO: is this valid?
        /*
            // Flush the command buffer
            glFlush();

            // Create an OpenGL sync object
            GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
            if (!sync)
            {
                // Handle error if sync creation failed
                // For example: CT_CORE_ASSERT(false, "Failed to create sync object.");
                return;
            }

            // Poll for sync completion
            GLenum result = GL_UNSIGNALED;
            while (result != GL_CONDITION_SATISFIED && result != GL_ALREADY_SIGNALED)
            {
                result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
                
                // Sleep for a short duration to avoid aggressive polling
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            // Cleanup the sync object
            glDeleteSync(sync);
        */
    }

    GPUInfo GLDevice::GetCurrentGPUInfo()
    {
        return GPUInfo();
    }
}