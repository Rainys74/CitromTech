#pragma once

#include "Core.h"

#include "CTL/String.h"
#include "CTL/DArray.h"

namespace Citrom
{
	// Responsible for transpiling and caching shaders
	namespace ShaderCompiler
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
	}
}