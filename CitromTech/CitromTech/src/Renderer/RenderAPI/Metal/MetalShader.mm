#include "MetalDevice.h"
#include "CitromAssert.h"

#include <fstream>

namespace Citrom::RenderAPI
{
    static std::string GetShaderFileName(const std::string& shaderName, ShaderType shaderFormat)
    {
        std::string filePath(shaderName);
        filePath.insert(0, "ShaderCache/");
        
        switch (shaderFormat)
        {
            case ShaderType::Vertex:
                filePath.append("_vs");
                break;
            case ShaderType::Fragment:
                filePath.append("_fs");
                break;
            default:
                CT_CORE_ASSERT(false, "Unsupported Shader Format!");
                break;
        }

        filePath.append(".msl");

        return filePath;
    }

        static id<MTLFunction> GetMetalShaderMainFunction(id<MTLLibrary> library)
        {
            // Ensure the library exists
            if (!library)
            {
                NSLog(@"Error: Metal library is nil.");
                return nil;
            }

            // List the available function names in the library
            NSArray* functionNames = [library functionNames];
            if (functionNames.count == 0)
            {
                NSLog(@"Error: No functions found in the Metal library.");
                return nil;
            }

            // Loop through all function names and look for a function that contains 'main' or 'Main'
            for (NSString* functionName in functionNames)
            {
                if ([functionName.lowercaseString containsString:@"main"])
                {
                    NSLog(@"Found shader function with 'main' in the name: %@", functionName);
                    return [library newFunctionWithName:functionName];
                }
            }

            // If no matching function name found, log an error
            NSLog(@"Error: No function found containing 'main' in the name. Available functions: %@", functionNames);
            return nil;
        }

        Shader MetalDevice::CreateShader(ShaderDesc* descriptor)
        {
            CREATE_BUFFER_INTERNAL(Shader, ShaderMTL, shader, internalData);
            
            std::string rawVertexSourceCode;
            {
                std::ifstream file;
                file.open(GetShaderFileName(descriptor->name, ShaderType::Vertex));
                std::stringstream reader;
                reader << file.rdbuf();
                rawVertexSourceCode = reader.str();
            }
            std::string rawFragmentSourceCode;
            {
                std::ifstream file;
                file.open(GetShaderFileName(descriptor->name, ShaderType::Fragment));
                std::stringstream reader;
                reader << file.rdbuf();
                rawFragmentSourceCode = reader.str();
                rawFragmentSourceCode.insert(0, "#pragma clang diagnostic push\n" "#pragma clang diagnostic ignored \"-Wambiguous-call-to-member-function\" \n");
            }
            
            NSString* vertexSource = [NSString stringWithUTF8String:rawVertexSourceCode.c_str()];
            NSString* fragSource = [NSString stringWithUTF8String:rawFragmentSourceCode.c_str()];
            
            NSError* error = nil;
            id<MTLLibrary> library = [m_Device newLibraryWithSource:vertexSource options:nil error:&error];
            NSLog(@"Error Status after compiling vertex shader: %@", error);
            CT_CORE_ASSERT(error == nil, "Error compiling Vertex Shader!");
            
            internalData->vertexFunction = GetMetalShaderMainFunction(library);
            [library release];
            
            library = [m_Device newLibraryWithSource:fragSource options:nil error:&error];
            NSLog(@"Error Status after compiling fragment shader: %@", error);
            CT_CORE_ASSERT(error == nil, "Error compiling Fragment Shader!");
            
            internalData->fragmentFunction = GetMetalShaderMainFunction(library);
            [library release];
            
            return shader;
        }
}
