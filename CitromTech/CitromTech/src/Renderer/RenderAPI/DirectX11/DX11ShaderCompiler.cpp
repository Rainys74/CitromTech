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

#if 0
	static std::wstring ReadShaderFromFileW(const std::filesystem::directory_entry& entry)
	{
		std::wifstream fileStream(entry.path());
		CT_CORE_ASSERT(fileStream, "Failed to open shader file ({}) for reading!", entry.path().string());

		std::wstringstream buffer;
		buffer << L"#define SHADER_LANG_HLSL" << L'\n'; // CT_LANG_HLSL, SHADER_LANGUAGE_HLSL, INITIAL_SHADER_LANG_HLSL
		buffer << fileStream.rdbuf();

		return buffer.str();
	}
#define ReadShaderFromFile_Type std::wstring
#define ReadShaderFromFile ReadShaderFromFileW
#else
	static std::string ReadShaderFromFileA(const std::filesystem::directory_entry& entry)
	{
		std::ifstream fileStream(entry.path());
		CT_CORE_ASSERT(fileStream, "Failed to open shader file ({}) for reading!", entry.path().string());

		std::stringstream buffer;
		buffer << "#define INITIAL_SHADER_LANG_HLSL" << '\n'; // CT_LANG_HLSL, SHADER_LANGUAGE_HLSL, INITIAL_SHADER_LANG_HLSL, SHADER_LANG_HLSL
		buffer << fileStream.rdbuf();

		return buffer.str();
	}
#define ReadShaderFromFile_Type std::string
#define ReadShaderFromFile ReadShaderFromFileA
#endif

	/*class ShaderIncludeHandler : public ID3DInclude
	{
	public:
		ShaderIncludeHandler(const std::wstring& shaderDirectory)
			: m_ShaderDirectory(shaderDirectory) {}

		STDMETHOD(Open)(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override 
		{
			std::wstring includePath;
			if (pParentData) 
			{
				std::wstring parentPath = *reinterpret_cast<const std::wstring*>(pParentData);
				includePath = std::filesystem::path(parentPath).parent_path() / std::wstring(pFileName, pFileName + strlen(pFileName));
			}
			else 
			{
				includePath = m_ShaderDirectory + std::wstring(pFileName, pFileName + strlen(pFileName));
			}

			std::ifstream fileStream(includePath, std::ios::binary);
			if (!fileStream) 
				return E_FAIL;

			fileStream.seekg(0, std::ios::end);
			size_t fileSize = fileStream.tellg();
			fileStream.seekg(0, std::ios::beg);

			BYTE* fileData = new BYTE[fileSize];
			fileStream.read(reinterpret_cast<char*>(fileData), fileSize);

			*ppData = fileData;
			*pBytes = static_cast<UINT>(fileSize);

			// Store the parent path for resolving nested includes
			*reinterpret_cast<const std::wstring**>(&pParentData) = new std::wstring(includePath);

			return S_OK;
		}

		STDMETHOD(Close)(LPCVOID pData) override 
		{
			delete[] pData;
			delete reinterpret_cast<const std::wstring*>(pData);
			return S_OK;
		}

	private:
		std::wstring m_ShaderDirectory;
	};*/
	class ShaderIncludeHandler : public ID3DInclude
	{
		HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
		{
			std::string filePath;
			switch (IncludeType)
			{
				case D3D_INCLUDE_LOCAL: // e.g. #include "test.h"
					filePath = std::string("Shaders/" + std::string(pFileName));
					break;
				case D3D_INCLUDE_SYSTEM: // e.g. #include <test.h>
					filePath = std::string("Shaders/" + std::string(pFileName)); // TODO: add the total/absolute path to the shader directory
					break;

				default:
					return E_FAIL;
					break;
			}

			std::ifstream fileStream(filePath);
			if (!fileStream || !fileStream.is_open())
				return E_FAIL;

			std::stringstream buffer;
			buffer << fileStream.rdbuf();

			*pBytes = static_cast<UINT>(buffer.str().size()/* + 1 */);

			uint8* data = (uint8*)Memory::Allocate(*pBytes);
			if (!data)
				return E_OUTOFMEMORY;
			Memory::Copy(data, buffer.str().c_str(), buffer.str().length()/* + 1*/);

			*ppData = data;

			return S_OK;
		}

		HRESULT Close(LPCVOID pData) override
		{
			Memory::Free(const_cast<void*>(pData));
			return S_OK;
		}
	};

#if 0
#define COMPILE_SHADER_HELPER(ENTRY, DEFINES, INCLUDE, ENTRYPOINT, TARGET, FLAGS1, FLAGS2, CODE, ERRORMSGS) DXCallErrorBlobHR(D3DCompileFromFile((ENTRY).path().wstring().c_str(), DEFINES, INCLUDE, ENTRYPOINT, TARGET, FLAGS1, FLAGS2, CODE, ERRORMSGS))
#else
#define COMPILE_SHADER_HELPER(ENTRY, DEFINES, INCLUDE, ENTRYPOINT, TARGET, FLAGS1, FLAGS2, CODE, ERRORMSGS) {												\
	ReadShaderFromFile_Type _intern_shaderSource = ReadShaderFromFile(ENTRY);																							\
	DXCallErrorBlobHR(D3DCompile(_intern_shaderSource.data(), _intern_shaderSource.size(), (ENTRY).path().stem().string().c_str(), DEFINES, INCLUDE, ENTRYPOINT, TARGET, FLAGS1, FLAGS2, CODE, ERRORMSGS))			\
	}
#endif

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
					ShaderObj computeShaderObject;

					vertexShaderObject.type = RenderAPI::ShaderType::Vertex;
					pixelShaderObject.type = RenderAPI::ShaderType::Fragment;
					computeShaderObject.type = RenderAPI::ShaderType::Compute;

					HRESULT hr;

					ID3DBlob* errorBlob = nullptr;

					ShaderIncludeHandler include;
					if (entry.path().stem().string().find("_vs") != std::string::npos)
					{
						vertexShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Separated HLSL Vertex Shader {}", vertexShaderObject.name);
						
						COMPILE_SHADER_HELPER(entry, nullptr, &include, "main", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob);

						//std::wstring shaderSource = ReadShaderFromFileW(entry);
						//D3DCompile(shaderSource.data(), shaderSource.size(), entry.path().stem().string().c_str(), nullptr, nullptr, "main", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob);
					}
					else if (entry.path().stem().string().find("_fs") != std::string::npos)
					{
						pixelShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Separated HLSL Pixel Shader {}", pixelShaderObject.name);

						COMPILE_SHADER_HELPER(entry, nullptr, &include, "main", "ps_5_0", NULL, NULL, &pixelShaderObject.shaderBlob, &errorBlob);
					}
					else if (entry.path().stem().string().find("_cs") != std::string::npos)
					{
						computeShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found HLSL Compute Shader {}", computeShaderObject.name);

						COMPILE_SHADER_HELPER(entry, nullptr, &include, "main", "cs_5_0", NULL, NULL, &computeShaderObject.shaderBlob, &errorBlob);
					}
					else if (entry.path().stem().string().find("_h") == std::string::npos /* && entry.path().extension() != ".hhlsl"*/) // this extension already is not computed in this block of code // x_hlsl.h
					{
						vertexShaderObject.name = pixelShaderObject.name = entry.path().stem().string();
						CT_CORE_TRACE("Found Combined (Vertex/Pixel) HLSL Shader {}", vertexShaderObject.name);

						COMPILE_SHADER_HELPER(entry, nullptr, &include, "vsmain", "vs_5_0", NULL, NULL, &vertexShaderObject.shaderBlob, &errorBlob);
						COMPILE_SHADER_HELPER(entry, nullptr, &include, "psmain", "ps_5_0", NULL, NULL, &pixelShaderObject.shaderBlob, &errorBlob);
					}
					else
					{
						CT_CORE_ASSERT(false, "Compiling DX11 shaders has resulted in undefined behaviour!");
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
					if (computeShaderObject.shaderBlob)
					{
						CT_CORE_INFO("Successfully Compiled DX11 Compute Shader {}", computeShaderObject.name);
						shaderObjects.PushBack(computeShaderObject);
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