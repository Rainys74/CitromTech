#include "DX11ShaderCompiler.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#include <filesystem>
#include <fstream>

#include "Renderer/RenderAPI/Graphics.h"
#include "CitromAssert.h"

#include "CTL/DArray.h"

#define DXCallErrorBlobHR(x) DXCallHR(x); if (errorBlob) {MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "D3DCompileFromFile Error Blob!", MB_ICONERROR);}

namespace Citrom::ShaderCompiler::DX11
{
	struct ShaderObj
	{
		std::string name;
		RenderAPI::ShaderType type;
		//void* shaderBlob;
		ID3DBlob* shaderBlob = nullptr;
	};

	static CTL::DArray<ShaderObj> CompileShadersToBlobs(const std::string shaderPaths[], const uint32 pathCount)
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

					vertexShaderObject.type = RenderAPI::ShaderType::Vertex;
					pixelShaderObject.type = RenderAPI::ShaderType::Fragment;

					HRESULT hr;

					ID3DBlob* errorBlob = nullptr;

					if (entry.path().stem().string().find("_vs") != std::string::npos)
					{
						vertexShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Separated HLSL Vertex Shader {}", vertexShaderObject.name);

						DXCallErrorBlobHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "main", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob));
					}
					else if (entry.path().stem().string().find("_fs") != std::string::npos)
					{
						pixelShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Separated HLSL Pixel Shader {}", pixelShaderObject.name);

						DXCallErrorBlobHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "main", "ps_5_0", NULL, NULL, &pixelShaderObject.shaderBlob, &errorBlob));
					}
					else
					{
						vertexShaderObject.name = pixelShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Combined (Vertex/Pixel) HLSL Shader {}", vertexShaderObject.name);

						DXCallErrorBlobHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob));
						DXCallErrorBlobHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &pixelShaderObject.shaderBlob, &errorBlob));
					}

					if (vertexShaderObject.shaderBlob)
					{
						CT_CORE_INFO("Successfully Compiled DX11 Vertex Shader {}", vertexShaderObject.name);
						shaderObjects.PushBack(vertexShaderObject);
					}
					if (pixelShaderObject.shaderBlob)
					{
						CT_CORE_INFO("Successfully Compiled DX11 Pixel Shader {}", pixelShaderObject.name);
						shaderObjects.PushBack(pixelShaderObject);
					}

					/*ShaderObj vertexShaderObject;
					ShaderObj pixelShaderObject;

					vertexShaderObject.name = pixelShaderObject.name = entry.path().stem().string();

					vertexShaderObject.type = RenderAPI::ShaderType::Vertex;
					pixelShaderObject.type = RenderAPI::ShaderType::Fragment;

					HRESULT hr;

					ID3DBlob* errorBlob = nullptr;
					DXCallErrorBlobHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob));
					DXCallErrorBlobHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &pixelShaderObject.shaderBlob, &errorBlob));

					shaderObjects.PushBack(vertexShaderObject);
					shaderObjects.PushBack(pixelShaderObject);*/
				}
			}
		}

		return shaderObjects;
	}
	void CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath)
	{
		//CT_PROFILE_GLOBAL_FUNCTION();

		CTL::DArray<ShaderObj> shaderObjects = CompileShadersToBlobs(shaderPaths, pathCount);

		for (const ShaderObj& shaderObj : shaderObjects)
		{	
			std::string shaderTypePrefix(""); //("_xx");
			if (shaderObj.name.find("_vs") == std::string::npos && shaderObj.name.find("_fs") == std::string::npos)
			{
				switch (shaderObj.type)
				{
					case RenderAPI::ShaderType::Vertex:
						shaderTypePrefix = "_vs";
						break;
					case RenderAPI::ShaderType::Fragment:
						shaderTypePrefix = "_fs";
						break;
					default: CT_CORE_ASSERT(false, "Invalid Shader Type!"); break;
				}
			}

			// DXBC - DirectX Byte-Code (HLSL5); CSO - Compiled Shader Object; DXIL - DirectX Intermediate Language (HLSL6);
			std::ofstream file(outPath + shaderObj.name + shaderTypePrefix + ".dxbc", std::ios::binary);
			CT_CORE_ASSERT(file.is_open(), "Could not open file stream!");

			file.write(reinterpret_cast<char*>(shaderObj.shaderBlob->GetBufferPointer()), shaderObj.shaderBlob->GetBufferSize());
			file.close();
			
			shaderObj.shaderBlob->Release();

			CT_CORE_INFO("Successfully wrote Shader {} to file: {}", shaderObj.name, outPath + shaderObj.name + shaderTypePrefix + ".dxbc");
		}
	}
}
#endif