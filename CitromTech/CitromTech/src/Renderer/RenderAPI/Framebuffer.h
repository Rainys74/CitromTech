#pragma once

#include "Graphics.h"
#include "CTL/RefPointer.h"
#include "CTL/DArray.h"

namespace Citrom::RenderAPI
{
	enum class FramebufferFormat
	{
		None,

		RGBA8,
		DEPTH24STENCIL8,
		R32F
	};

	struct FramebufferDesc
	{
		uint32 width = 0, height = 0; // 0 will take the screen's size
		uint32 samples = 1;

		FramebufferFormat format;
	};

	struct Framebuffer
	{
		FramebufferDesc descriptor;
		CTL::Ref<void> internal;
	};
}