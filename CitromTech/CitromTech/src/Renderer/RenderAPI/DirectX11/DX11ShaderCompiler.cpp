#include "DX11ShaderCompiler.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#include <filesystem>

namespace Citrom::ShaderCompiler::DX11
{
	CTL::DArray<ShaderObj> CompileShaders(const std::string shaderPaths[], const uint32 pathCount, const std::string& outPath)
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

					vertexShaderObject.name = pixelShaderObject.name = entry.path().string();

					vertexShaderObject.type = RenderAPI::ShaderType::Vertex;
					pixelShaderObject.type = RenderAPI::ShaderType::Fragment;

					HRESULT hr;

					ID3DBlob* errorBlob = nullptr;
					DXCallHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob));
					DXCallHR(D3DCompileFromFile(entry.path().wstring().c_str(), nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &pixelShaderObject.shaderBlob, &errorBlob));

					shaderObjects.PushBack(vertexShaderObject);
					shaderObjects.PushBack(pixelShaderObject);
				}
			}
		}

		return shaderObjects;
	}
}
#endif