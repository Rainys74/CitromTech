#include "DX11ShaderCompiler.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#include <filesystem>
#include <fstream>

#include "Renderer/RenderAPI/Graphics.h"
#include "CitromAssert.h"

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
					ShaderObj vertexShaderObject;
					ShaderObj pixelShaderObject;

					vertexShaderObject.name = pixelShaderObject.name = entry.path().stem().string();

					vertexShaderObject.type = RenderAPI::ShaderType::Vertex;
					pixelShaderObject.type = RenderAPI::ShaderType::Fragment;

					HRESULT hr;

					ID3DBlob* errorBlob = nullptr;
					DXCallHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob));
					DXCallHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &pixelShaderObject.shaderBlob, &errorBlob));

					#ifndef CT_OPTIMIZATION
					if (errorBlob)
						MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "D3DCompileFromFile Error Blob!", MB_ICONERROR);
					#endif

					shaderObjects.PushBack(vertexShaderObject);
					shaderObjects.PushBack(pixelShaderObject);
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
			std::string shaderTypePrefix("_xx");
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

			// DXBC - DirectX Byte-Code; CSO - Compiled Shader Object; DXIL - DirectX Intermediate Language;
			std::ofstream file(outPath + shaderObj.name + shaderTypePrefix + ".dxbc", std::ios::binary);
			CT_CORE_ASSERT(file.is_open(), "Could not open file stream!");

			file.write(reinterpret_cast<char*>(shaderObj.shaderBlob->GetBufferPointer()), shaderObj.shaderBlob->GetBufferSize());
			file.close();
			
			shaderObj.shaderBlob->Release();
		}
	}
}
#endif