#include "ShaderCompiler.h"
#include "CitromAssert.h"

#include "ArgumentHandler.h"
#include "Platform/Platform.h"

#include <filesystem>

namespace Citrom
{
	namespace ShaderCompiler
	{
		static int (*glslcc_exec)(int argc, char* argv[]);

		void PrepareShaders(const std::string paths[], const uint32 pathCount, const std::string& outPath)
		{
			CT_PROFILE_GLOBAL_FUNCTION();

			Platform::DynamicLibrary glslcc("glslcc");
			//glslcc.Load("glslcc");

			glslcc_exec = (int(*)(int argc, char** argv))glslcc.GetProcedureAddress("glslcc_exec");

			for (const auto& entry : std::filesystem::recursive_directory_iterator(paths[0]))
			{
				if (entry.path().extension() == ".glsl") 
				{
					CT_VERBOSE("{}", entry.path().string());
				}
			}

			const char* arguments[] =
			{
				ArgumentHandler::GetFilePath(),
				"--vert=",
				"--frag=",
				"--output=",
				"--lang="

				//"--help"
			};
			{
				CT_PROFILE_SCOPE("glslcc_exec");

				int result = glslcc_exec(1, const_cast<char**>(arguments));
				CT_CORE_ASSERT(result, "glslcc_exec main function failed!");
			}
		}
	}
}
