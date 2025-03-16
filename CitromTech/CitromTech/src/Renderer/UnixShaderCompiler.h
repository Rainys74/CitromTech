#pragma once

#include "Core.h"

#ifdef CT_PLATFORM_UNIX
#include "CTL/String.h"

namespace Citrom::ShaderCompiler::Unix
{
	bool CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath);
}
#endif
