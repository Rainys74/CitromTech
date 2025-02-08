#include "Platform/Platform.h"

#ifdef CT_PLATFORM_MACOS
#include "CitromAssert.h"

#include "CTL/CStringHandling.h"

#include <cstdio>
#include <dlfcn.h>

#define DL_VERIFY(x) if (!(x)) { CT_CORE_ERROR("dl error: {}", dlerror()); CT_CORE_ASSERT(false, "Failed to " #x "!");}
#define DL_VERIFY_HANDLE(x) if (!(x)) { CT_CORE_ERROR("dl error: {}", dlerror()); CT_CORE_ASSERT(false, "Failed to load Dynamic Library!");}

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
        if (m_Handle)
            CT_CORE_VERIFY(dlclose(m_Handle), "Failed to free dynamic library!");
    }

    void DynamicLibrary::Load(const char* libraryName)
    {
        const size_t libPathLength = CTL::CString::GetLength(libraryName) + 3 + 6 + 1; // 3 - prefix length, 6 - suffix length, 2 - null terminators

        char* libraryPath = new char[libPathLength];
        std::snprintf(libraryPath, libPathLength, "%s%s%s", GetLibraryNamePrefix(), libraryName, GetLibraryNameSuffix());

        LoadEx(libraryPath);

        delete[] libraryPath;
    }

    void DynamicLibrary::LoadEx(const char* libraryPath)
    {
        m_Handle = dlopen(libraryPath, RTLD_NOW); // RTLD_LAZY only resolves symbols when they are actually used
        DL_VERIFY_HANDLE(m_Handle);
    }

    void* DynamicLibrary::GetProcedureAddress(const char* functionName)
    {
        void* address = dlsym(m_Handle, functionName);
        CT_CORE_ASSERT(address, "Couldn't retrieve function pointer address!");
        return address;
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
