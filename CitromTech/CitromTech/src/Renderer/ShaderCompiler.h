#pragma once

#include "Core.h"

#include "CTL/String.h"
#include "CTL/DArray.h"

namespace Citrom
{
	// Responsible for transpiling and caching shaders
	/*namespace ShaderCompiler
	{
		struct ShaderObj
		{
			//FILE file;
			//Type type;
			//std::string name;
		};

		// Transpiles shaders to different files using glslcc
		CTL::DArray<ShaderObj> PrepareShaders(const std::string paths[], const std::string& outPath){}

		//void GFXCompileShaders()

		void RecompileShaders(const std::string paths[], const std::string& outPath);
	}*/

	namespace ShaderCompiler
	{
		void ClearShaderCache(const std::string& cachePath); //const std::string cachePaths[]

		void PrepareShaders(const std::string paths[], const uint32 pathCount, const std::string& outPath);

		void CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath);
	}
}