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

		static void ExecuteGLSLCC(int argc, char** argv)
		{
			CT_PROFILE_SCOPE("glslcc_exec");

			int result = glslcc_exec(argc, argv);
			CT_CORE_ASSERT(!result, "glslcc_exec main function failed!");
		}

		void PrepareShaders(const std::string paths[], const uint32 pathCount, const std::string& outPath)
		{
			CT_PROFILE_GLOBAL_FUNCTION();

			Platform::DynamicLibrary glslcc("glslcc");
			//glslcc.Load("glslcc");

			glslcc_exec = (int(*)(int argc, char** argv))glslcc.GetProcedureAddress("glslcc_exec");

			std::string arguments[] =
			{
				ArgumentHandler::GetFilePath(),
				"--vert=",
				"--frag=",
				"--output=",
				"--lang="
			};
			for (const auto& entry : std::filesystem::recursive_directory_iterator(paths[0]))
			{
				if (!entry.is_regular_file())
					continue; // Skip non-file entries like directories

				CT_VERBOSE("{}", entry.path().string());
				if (entry.path().extension() == ".glsl")
				{
					//arguments[1] = std::string("--vert=").append(entry.path().string());
					//arguments[2] = std::string("--frag=").append(entry.path().string());
					arguments[1] = std::string(entry.path().string());
					arguments[2] = std::string("0");

					// HLSL
					arguments[3] = std::string("--output=").append(outPath).append(entry.path().stem().string()).append(".hlsl");
					arguments[4] = std::string("--lang=").append("hlsl");

					// Execute GLSLcc
					CTL::DArray<const char*> argDArray(CT_ARRAY_LENGTH(arguments));
					for (const std::string& arg : arguments)
					{
						argDArray.PushBack(arg.c_str());
					}

					// TODO: maybe use thread pool for this?
					// HLSL
					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));

					// GLSL
					arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".glsl";
					arguments[4] = "--lang=" "glsl";
					argDArray[3] = arguments[3].c_str();
					argDArray[4] = arguments[4].c_str();

					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));

					// MSL (Metal)
					arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".msl";
					arguments[4] = "--lang=" "msl";
					argDArray[3] = arguments[3].c_str();
					argDArray[4] = arguments[4].c_str();

					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
				}
				else if (entry.path().extension() == ".vert")
				{
					arguments[1] = std::string("--vert=").append(entry.path().string());
					arguments[2] = std::string("0");

					// HLSL
					arguments[3] = std::string("--output=").append(outPath).append(entry.path().stem().string()).append(".hlsl");
					arguments[4] = std::string("--lang=").append("hlsl");

					// Execute GLSLcc
					CTL::DArray<const char*> argDArray(CT_ARRAY_LENGTH(arguments));
					for (const std::string& arg : arguments)
					{
						argDArray.PushBack(arg.c_str());
					}

					// TODO: maybe use thread pool for this?
					// HLSL
					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));

					// GLSL
					arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".glsl";
					arguments[4] = "--lang=" "glsl";
					argDArray[3] = arguments[3].c_str();
					argDArray[4] = arguments[4].c_str();

					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));

					// MSL (Metal)
					arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".msl";
					arguments[4] = "--lang=" "msl";
					argDArray[3] = arguments[3].c_str();
					argDArray[4] = arguments[4].c_str();

					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
				}
				else if (entry.path().extension() == ".frag")
				{
					arguments[1] = std::string("0");
					arguments[2] = std::string("--frag=").append(entry.path().string());

					// HLSL
					arguments[3] = std::string("--output=").append(outPath).append(entry.path().stem().string()).append(".hlsl");
					arguments[4] = std::string("--lang=").append("hlsl");

					// Execute GLSLcc
					CTL::DArray<const char*> argDArray(CT_ARRAY_LENGTH(arguments));
					for (const std::string& arg : arguments)
					{
						argDArray.PushBack(arg.c_str());
					}

					// TODO: maybe use thread pool for this?
					// HLSL
					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));

					// GLSL
					arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".glsl";
					arguments[4] = "--lang=" "glsl";
					argDArray[3] = arguments[3].c_str();
					argDArray[4] = arguments[4].c_str();

					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));

					// MSL (Metal)
					arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".msl";
					arguments[4] = "--lang=" "msl";
					argDArray[3] = arguments[3].c_str();
					argDArray[4] = arguments[4].c_str();

					ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
				}
				else if (entry.path().extension() == ".comp")
				{

				}
				else continue;

				// glslcc execute
			}

			const char* argumentsTest[] =
			{
				ArgumentHandler::GetFilePath(),
				"--help"
			};
			{
				CT_PROFILE_SCOPE("glslcc_exec");

				int result = glslcc_exec(1, const_cast<char**>(argumentsTest));
				CT_CORE_ASSERT(result, "glslcc_exec main function failed!");
			}
		}
	}
}
