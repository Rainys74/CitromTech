#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"

namespace Citrom::RenderAPI
{
	struct ShaderDesc
	{
		std::string name;
	};

	struct Shader
	{
		ShaderDesc descriptor;
		CTL::Ref<void> internal;
	};
}