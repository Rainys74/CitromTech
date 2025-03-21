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
#elif defined(CT_PLATFORM_UNIX)
#include "UnixShaderCompiler.h"
#endif

namespace Citrom
{
	namespace ShaderCompiler
	{
		static int (*glslcc_exec)(int argc, char* argv[]);

		static void ExecuteGLSLCC(int argc, char** argv)
		{
			CT_PROFILE_SCOPE("glslcc_exec");

#ifdef CT_DEBUG
			CT_CORE_TRACE("Executing GLSLcc with args ({}):", argc);
			for (int i = 0; i < argc; i++)
				CT_CORE_TRACE("{}. {}{}", i, argv[i], i == (argc-1) ? "\n" : "");
#endif

			int result = glslcc_exec(argc, argv);
			CT_CORE_ASSERT(!result, "glslcc_exec main function failed!");
		}
		static void TranspileGLSLCC(const std::string paths[], const uint32 pathCount, const std::string& outPath, RenderAPI::ShaderLanguage affectedLanguages = RenderAPI::ShaderLanguage::All)
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
				"--lang=",

				// TODO: test these 2 out
				"--defines=INITIAL_SHADER_LANG_GLSL",
				"--include-dirs=Shaders;Shaders/ShaderInterop"//,
				//"--optimize" // Does this even have an effect on non-binary files?
			};
			for (uint32 i = 0; i < pathCount; i++)
			{
				for (const auto& entry : std::filesystem::recursive_directory_iterator(paths[i]))
				{
					if (!entry.is_regular_file())
						continue; // Skip non-file entries like directories

					//CT_WARN("ENTRY:PATH:FILENAME: {}", entry.path().filename().string()); // TextureShaderCode.glsl
					//CT_WARN("ENTRY:PATH:EXTENSION: {}", entry.path().extension().string()); // .glsl
					//CT_WARN("ENTRY:PATH:STEM: {}", entry.path().stem().string()); // TextureShaderCode

					// TODO: Refactor since this looks retarted
					if (entry.path().extension() == ".glsl" && entry.path().stem().string().ends_with("_vs"))
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
						{
							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}

						// GLSL
						{
							arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".glsl";
							arguments[4] = "--lang=" "glsl";
							argDArray[3] = arguments[3].c_str();
							argDArray[4] = arguments[4].c_str();

							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}

						// MSL (Metal)
						{
							arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".msl";
							arguments[4] = "--lang=" "msl";
							argDArray[3] = arguments[3].c_str();
							argDArray[4] = arguments[4].c_str();

							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}
					}
					else if (entry.path().extension() == ".glsl" && entry.path().stem().string().ends_with("_fs"))
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
					else if (entry.path().extension() == ".glsl" 
						&& entry.path().stem().string().ends_with("_cs"))
						//&& entry.path().filename().string().find("_cs") != std::string::npos)
					{
						arguments[1] = std::string("--compute=").append(entry.path().string());
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
						{
							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}

						// GLSL
						{
							arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".glsl";
							arguments[4] = "--lang=" "glsl";
							argDArray[3] = arguments[3].c_str();
							argDArray[4] = arguments[4].c_str();

							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}

						// MSL (Metal)
						{
							arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".msl";
							arguments[4] = "--lang=" "msl";
							argDArray[3] = arguments[3].c_str();
							argDArray[4] = arguments[4].c_str();

							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}
					}
					else if (entry.path().extension() == ".glsl")
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
						if (HAS_FLAG(affectedLanguages, RenderAPI::ShaderLanguage::HLSL))
						{
							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}
						// GLSL
						if (HAS_FLAG(affectedLanguages, RenderAPI::ShaderLanguage::GLSL))
						{
							arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".glsl";
							arguments[4] = "--lang=" "glsl";
							argDArray[3] = arguments[3].c_str();
							argDArray[4] = arguments[4].c_str();

							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}

						// MSL (Metal)
						if (HAS_FLAG(affectedLanguages, RenderAPI::ShaderLanguage::MSL))
						{
							arguments[3] = "--output=" + outPath + entry.path().stem().string() + ".msl";
							arguments[4] = "--lang=" "msl";
							argDArray[3] = arguments[3].c_str();
							argDArray[4] = arguments[4].c_str();

							ExecuteGLSLCC(CT_ARRAY_LENGTH(arguments), const_cast<char**>(argDArray.Data()));
						}
					}
					else continue;

					// glslcc execute
				}
			}
		}
		static void TranspileHLSLcc(const std::string paths[], const uint32 pathCount, const std::string& outPath, RenderAPI::ShaderLanguage affectedLanguages = RenderAPI::ShaderLanguage::All)
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
                        
                        //static std::string metalEntryPointName;
                        //class MetalReflection : public HLSLccReflection
                        //{
                        //    void OnVertexProgramOutput(const std::string& name, const std::string& semantic, int semanticIndex) override
                        //    {
                        //        //std::cout << "Vertex function: " << name << std::endl;
                        //        metalEntryPointName = name;
                        //    }
                        //    void OnFragmentOutputDeclaration(int numComponents, int outputIndex) override
                        //    {
                        //        //std::cout << "Fragment function index: " << outputIndex << std::endl;
                        //    }
                        //} metalReflectionCallback;
						
						// GLSL
						TranslateHLSLFromFile(entry.path().string().c_str(), HLSLCC_FLAG_UNIFORM_BUFFER_OBJECT, LANG_GL_LAST, &glExtensions, nullptr, samplerPrecision, reflectionCallback, &glResult);

						// Metal
						TranslateHLSLFromFile(entry.path().string().c_str(), 0x00000000, LANG_METAL, &glExtensions, nullptr, samplerPrecision, reflectionCallback, &metalResult);

						// GLSL
						if (HAS_FLAG(affectedLanguages, RenderAPI::ShaderLanguage::GLSL))
						{
							std::ofstream outFile(outPath + entry.path().stem().string() + ".glsl", std::ios::out);
							CT_CORE_ASSERT(outFile.is_open(), "Failed to open file for writing GLSL source code!");

							outFile << glResult.sourceCode;

							outFile.close();

							CT_CORE_TRACE("Successfully transpiled DXBC file: ({}) to {}", entry.path().string().c_str(), /*outPath +*/ entry.path().stem().string() + ".glsl");
						}

						// Metal Shading Language
						if (HAS_FLAG(affectedLanguages, RenderAPI::ShaderLanguage::MSL))
						{
							std::ofstream outFile(outPath + entry.path().stem().string() + ".msl", std::ios::out);
							CT_CORE_ASSERT(outFile.is_open(), "Failed to open file for writing MSL source code!");

							outFile << metalResult.sourceCode;

							outFile.close();

							CT_CORE_TRACE("Successfully transpiled DXBC file: ({}) to {}", entry.path().string().c_str(), /*outPath +*/ entry.path().stem().string() + ".msl");
						}
					}
				}
			}
		}

		void ClearShaderCache(const std::string& cachePath)
		{
			CT_PROFILE_GLOBAL_FUNCTION();

			static const std::string validExtensions[] =
			{
				".glsl",
				".hlsl",
				".msl",
				".dxbc"
			};

			for (const auto& entry : std::filesystem::recursive_directory_iterator(cachePath))
			{
				if (!entry.is_regular_file())
					continue; // Skip non-file entries like directories

				for (const auto& extension : validExtensions)
				{
					if (extension == entry.path().extension().string())
					{
						CT_CORE_TRACE("Deleting Cached Shader in: {}", entry.path().string());
						std::filesystem::remove(entry.path());
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

			/*switch (RenderAPI::GraphicsAPIManager::GetGraphicsAPI())
			{
				case RenderAPI::GraphicsAPI::DirectX11:
					IF_WINDOWS(ShaderCompiler::DX11::CompileShaders(shaderPaths, pathCount, outPath));
					break;
				default: CT_CORE_ASSERT(false, "Current Graphics API is invalid!"); break;
			}*/
            IF_WINDOWS(ShaderCompiler::DX11::CompileShaders(shaderPaths, pathCount, outPath));
            // bool result = ShaderCompiler::Unix::CompileShaders(); // use wine probably
            #ifdef CT_PLATFORM_UNIX
            if (!ShaderCompiler::Unix::CompileShaders(shaderPaths, pathCount, outPath))
                CT_CORE_WARN("Could not compile HLSL5 shaders to byte-code, if you need dynamic recompilation, please make sure Microsoft's FXC Compiler works correctly through Wine.");
            #endif

			TranspileHLSLcc(shaderPaths, pathCount, outPath);
		}
	}
}
