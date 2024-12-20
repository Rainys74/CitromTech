#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"
#include "CTL/DArray.h"

namespace Citrom::RenderAPI
{
	struct FramebufferDesc
	{
		uint32 width, height;
		uint32 samples = 1;
	};

	struct Framebuffer
	{
		FramebufferDesc descriptor;
		CTL::Ref<void> internal;
	};
}