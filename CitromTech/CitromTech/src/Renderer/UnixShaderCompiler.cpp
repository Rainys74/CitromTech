#include "UnixShaderCompiler.h"

#ifdef CT_PLATFORM_UNIX
#include <filesystem>
#include <fstream>

#include "Renderer/RenderAPI/Graphics.h"
#include "Platform/Platform.h"
#include "CitromAssert.h"

#include "CTL/DArray.h"


namespace Citrom::ShaderCompiler::Unix
{
    struct ShaderObj
    {
        std::string name;
        RenderAPI::ShaderType type;
        
        struct Arguments
        {
            Arguments() {}
            Arguments(const std::filesystem::directory_entry& _entry, const std::string& _entryPointName, const std::string& _shaderFormat)
                : entry(_entry), entryPointName(_entryPointName), shaderFormat(_shaderFormat) {}
            
            std::filesystem::directory_entry entry;
            std::string entryPointName;
            std::string shaderFormat;
        } arguments;
    };

#define CompileShader ShaderObj::Arguments

    static CTL::DArray<ShaderObj> GetShaderObjects(const std::string shaderPaths[], const uint32 pathCount)
	{
		CT_PROFILE_GLOBAL_FUNCTION();

		CTL::DArray<ShaderObj> shaderObjects;

		for (uint32 i = 0; i < pathCount; i++)
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(shaderPaths[i]))
			{
				if (!entry.is_regular_file())
					continue; // Skip non-file entries like directories

				if (entry.path().extension() == ".hlsl")
				{
					CT_CORE_TRACE("Found HLSL Shader in {}", entry.path().string());

					ShaderObj vertexShaderObject;
					ShaderObj pixelShaderObject;
					ShaderObj computeShaderObject;

					vertexShaderObject.type = RenderAPI::ShaderType::Vertex;
					pixelShaderObject.type = RenderAPI::ShaderType::Fragment;
					computeShaderObject.type = RenderAPI::ShaderType::Compute;

					if (entry.path().stem().string().find("_vs") != std::string::npos)
					{
						vertexShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Separated HLSL Vertex Shader {}", vertexShaderObject.name);
						
						vertexShaderObject.arguments = CompileShader(entry, "main", "vs_5_0");
					}
					else if (entry.path().stem().string().find("_fs") != std::string::npos)
					{
						pixelShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Separated HLSL Pixel Shader {}", pixelShaderObject.name);

                        pixelShaderObject.arguments = CompileShader(entry, "main", "ps_5_0");
					}
					else if (entry.path().stem().string().find("_cs") != std::string::npos)
					{
						computeShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found HLSL Compute Shader {}", computeShaderObject.name);

                        computeShaderObject.arguments = CompileShader(entry, "main", "cs_5_0");
					}
					else if (entry.path().stem().string().find("_h") == std::string::npos /* && entry.path().extension() != ".hhlsl"*/) // this extension already is not computed in this block of code // x_hlsl.h
					{
						vertexShaderObject.name = pixelShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Combined (Vertex/Pixel) HLSL Shader {}", vertexShaderObject.name);

                        vertexShaderObject.arguments = CompileShader(entry, "vsmain", "vs_5_0");
                        pixelShaderObject.arguments = CompileShader(entry, "psmain", "ps_5_0");
					}
					else
					{
						CT_CORE_ASSERT(false, "Compiling HLSL shaders has resulted in undefined behaviour!");
					}
                    
                    if (!vertexShaderObject.name.empty())
                        shaderObjects.PushBack(vertexShaderObject);
                    if (!vertexShaderObject.name.empty())
                        shaderObjects.PushBack(pixelShaderObject);
                    
                    if (!computeShaderObject.name.empty())
                        shaderObjects.PushBack(computeShaderObject);
				}
			}
		}

		return shaderObjects;
	}

    // Check if Wine is installed
    static bool IsWineAvailable()
    {
        int result = system("command -v wine > /dev/null 2>&1");  // Check if 'wine' exists
        return (result == 0);
    }
    static bool IsMicrosoftFXCAvailable()
    {
        int fxcCheck = system("wine fxc.exe /? > /dev/null 2>&1");
        return !(fxcCheck != 0);
    }

	bool CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath)
	{
		//CT_PROFILE_GLOBAL_FUNCTION();

		CTL::DArray<ShaderObj> shaderObjects = GetShaderObjects(shaderPaths, pathCount);

		for (const ShaderObj& shaderObj : shaderObjects)
        {
            std::string shaderTypePrefix(""); //("_xx");
            if (shaderObj.name.find("_vs") == std::string::npos && shaderObj.name.find("_fs") == std::string::npos && shaderObj.name.find("_cs") == std::string::npos)
            {
                switch (shaderObj.type)
                {
                    case RenderAPI::ShaderType::Vertex:
                        shaderTypePrefix = "_vs";
                        break;
                    case RenderAPI::ShaderType::Fragment:
                        shaderTypePrefix = "_fs";
                        break;
                    case RenderAPI::ShaderType::Compute:
                        shaderTypePrefix = "_cs";
                        break;
                    default: CT_CORE_ASSERT(false, "Invalid Shader Type!"); break;
                }
            }
            
            // DXBC - DirectX Byte-Code (HLSL5); CSO - Compiled Shader Object; DXIL - DirectX Intermediate Language (HLSL6);
            const std::string outputPath = Platform::Utils::GetWorkingDirectory() + "/" + outPath + shaderObj.name + shaderTypePrefix + ".dxbc";
            
            if (!IsWineAvailable())
            {
                CT_CORE_WARN("Wine is not installed or not in PATH.");
                return false;
            }
            if (!IsMicrosoftFXCAvailable())
            {
                CT_CORE_WARN("Wine is installed, but fxc.exe is not available.");
                return false;
            }
            
            const std::string command = "wine fxc.exe /T " + shaderObj.arguments.shaderFormat + " /E " + shaderObj.arguments.entryPointName + " /Fo " + outputPath;
            int result = system(command.c_str());
            if (!result)
            {
                CT_CORE_ERROR("Shader compilation failed via Wine.");
                return false;
            }
            
            CT_CORE_INFO("Successfully wrote Shader {} to file: {}", shaderObj.name, outputPath);
		}
        return true;
    }
}
#endif
