#pragma once

#include "Core.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"

#include "Renderer/RenderAPI/Graphics.h"

#include "CTL/String.h"
#include "CTL/DArray.h"

namespace Citrom::ShaderCompiler::DX11
{
	struct ShaderObj
	{
		std::string name;
		RenderAPI::ShaderType type;
		//void* shaderBlob;
		ID3DBlob* shaderBlob;
	};

	CTL::DArray<ShaderObj> CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath);
}
#endif