#include "Platform/Platform.h"

#ifdef CT_PLATFORM_MACOS
#include "CitromAssert.h"

#include "CTL/CStringHandling.h"

#include <cstdio>

namespace Citrom::Platform
{
    DynamicLibrary::DynamicLibrary()
        : m_Handle(nullptr)
    {
    }
    DynamicLibrary::DynamicLibrary(const char* libraryName)
        : m_Handle(nullptr)
    {
        Load(libraryName);
    }
    DynamicLibrary::~DynamicLibrary()
    {
    }

    void DynamicLibrary::Load(const char* libraryName)
    {

    }

    void DynamicLibrary::LoadEx(const char* libraryPath)
    {
   
    }

    void* DynamicLibrary::GetProcedureAddress(const char* functionName)
    {
        return nullptr;
    }

    constexpr const char* DynamicLibrary::GetLibraryNamePrefix()
    {
        return "lib";
    }

    constexpr const char* DynamicLibrary::GetLibraryNameSuffix()
    {
        return ".dylib";
    }
}
#endif