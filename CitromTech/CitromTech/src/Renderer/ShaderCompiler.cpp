#include "ShaderCompiler.h"
#include "CitromAssert.h"

#include "ArgumentHandler.h"
#include "Platform/Platform.h"

#include <filesystem>
#include <fstream>

#include "RenderAPI/GraphicsDevice.h"

#include "hlslcc.h"

#ifdef CT_PLATFORM_WINDOWS
#include "RenderAPI/DirectX11/DX11ShaderCompiler.h"
#endif

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
		static void TranspileGLSLCC(const std::string paths[], const uint32 pathCount, const std::string& outPath)
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
			for (uint32 i = 0; i < pathCount; i++)
			{
				for (const auto& entry : std::filesystem::recursive_directory_iterator(paths[i]))
				{
					if (!entry.is_regular_file())
						continue; // Skip non-file entries like directories

					// TODO: Refactor since this looks retarted
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
			}
		}
		static void TranspileHLSLcc(const std::string paths[], const uint32 pathCount, const std::string& outPath)
		{
			CT_PROFILE_GLOBAL_FUNCTION();

			//TranslateHLSLFromMem();

			for (uint32 i = 0; i < pathCount; i++)
			{
				for (const auto& entry : std::filesystem::recursive_directory_iterator(paths[i]))
				{
					if (!entry.is_regular_file())
						continue; // Skip non-file entries like directories

					if (entry.path().extension() == ".dxbc")
					{
						GLSLShader glResult;
						GLSLShader metalResult;

						GlExtensions glExtensions = {};

						HLSLccSamplerPrecisionInfo samplerPrecision = {};
						HLSLccReflection reflectionCallback = {};

						// HLSL
						TranslateHLSLFromFile(entry.path().string().c_str(), 0x00000000, LANG_GL_LAST, &glExtensions, nullptr, samplerPrecision, reflectionCallback, &glResult);
						// Metal
						TranslateHLSLFromFile(entry.path().string().c_str(), 0x00000000, LANG_METAL, &glExtensions, nullptr, samplerPrecision, reflectionCallback, &metalResult);

						// GLSL
						std::ofstream outFile(outPath + entry.path().stem().string() + ".glsl", std::ios::out);
						CT_CORE_ASSERT(outFile.is_open(), "Failed to open file for writing GLSL source code!");

						outFile << glResult.sourceCode;

						outFile.close();

						CT_CORE_TRACE("Successfully transpiled DXBC file: ({}) to {}", entry.path().string().c_str(), /*outPath +*/ entry.path().stem().string() + ".glsl");

						// Metal Shading Language
						std::ofstream outFile(outPath + entry.path().stem().string() + ".msl", std::ios::out);
						CT_CORE_ASSERT(outFile.is_open(), "Failed to open file for writing MSL source code!");

						outFile << metalResult.sourceCode;

						outFile.close();

						CT_CORE_TRACE("Successfully transpiled DXBC file: ({}) to {}", entry.path().string().c_str(), /*outPath +*/ entry.path().stem().string() + ".msl");
					}
				}
			}
		}

		void PrepareShaders(const std::string paths[], const uint32 pathCount, const std::string& outPath)
		{
			CT_PROFILE_GLOBAL_FUNCTION();

			TranspileGLSLCC(paths, pathCount, outPath);
		}

		void CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath)
		{
			CT_PROFILE_GLOBAL_FUNCTION();

			switch (RenderAPI::GraphicsAPIManager::GetGraphicsAPI())
			{
				case RenderAPI::GraphicsAPI::DirectX11:
					IF_WINDOWS(ShaderCompiler::DX11::CompileShaders(shaderPaths, pathCount, outPath));
					break;
				default: CT_CORE_ASSERT(false, "Current Graphics API is invalid!"); break;
			}

			TranspileHLSLcc(shaderPaths, pathCount, outPath);
		}
	}
}
