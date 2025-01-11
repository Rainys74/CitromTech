#pragma once

#include "Core.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"

#include "CTL/String.h"

namespace Citrom::ShaderCompiler::DX11
{
	void CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath);
}
#endif