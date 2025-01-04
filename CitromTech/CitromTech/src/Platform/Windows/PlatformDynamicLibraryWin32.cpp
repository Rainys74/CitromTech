#include "Platform/Platform.h"

#ifdef CT_PLATFORM_WINDOWS
#include "WindowsIncludes.h"
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
        if (m_Handle)
            CT_CORE_VERIFY(FreeLibrary((HMODULE)m_Handle), "Failed to free dynamic library!");
    }

    void DynamicLibrary::Load(const char* libraryName)
    {
        /*
        const uint16 libPathLength = cstring_get_length(libraryName) + 0 + 4 + 1; // 0 - prefix length, 4 - suffix length, 1 - null terminator
        char* libraryPath = memory_newarr(char, libPathLength);
        snprintf(libraryPath, (size_t)libPathLength, "%s%s%s", platform_dl_get_library_name_prefix(), libraryName, platform_dl_get_library_name_suffix());

        platform_dl* library = platform_dl_load_library_ex(libraryPath);

        memory_deletearr(libPathLength, libraryPath);

        return library;
        */
        const size_t libPathLength = CTL::CString::GetLength(libraryName) + 0 + 4 + 1; // 0 - prefix length, 4 - suffix length, 1 - null terminator

        char* libraryPath = new char[libPathLength];
        std::snprintf(libraryPath, libPathLength, "%s%s%s", GetLibraryNamePrefix(), libraryName, GetLibraryNameSuffix());

        LoadEx(libraryPath);

        delete[] libraryPath;
    }

    void DynamicLibrary::LoadEx(const char* libraryPath)
    {
        /*
        int bufferSize = MultiByteToWideChar(CP_UTF8, 0, libraryPath, -1, NULL, 0);
        CT_CORE_ASSERT(bufferSize != 0, "Failed to calculate buffer size!");

        wchar_t* wideString = memory_newarr(wchar_t, bufferSize);
        CT_CORE_ASSERT(MultiByteToWideChar(CP_UTF8, 0, libraryPath, -1, wideString, bufferSize), "Failed to convert string!");

        HMODULE library = LoadLibrary(wideString);

        memory_deletearr(bufferSize, wideString);

        //CT_CORE_ASSERT(library, "Failed to load Dynamic Library!");

        return library;
        */
        m_Handle = LoadLibraryA(libraryPath);
        
        CT_CORE_ASSERT(m_Handle, "Failed to load Dynamic Library!");
    }

    void* DynamicLibrary::GetProcedureAddress(const char* functionName)
    {
        void* address = GetProcAddress((HMODULE)m_Handle, functionName);
        CT_CORE_ASSERT(address, "Couldn't retrieve function pointer address!");
        return address; //return GetProcAddress((HMODULE)m_Handle, functionName);
    }

    constexpr const char* DynamicLibrary::GetLibraryNamePrefix()
    {
        return "";
    }

    constexpr const char* DynamicLibrary::GetLibraryNameSuffix()
    {
        return ".dll";
    }
}
#endif