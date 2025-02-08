#pragma once

#include "Core.h"
#include "CTL/String.h"

namespace Citrom::Platform
{
	namespace Utils
	{
		void Sleep(uint64 ms);
		int32 GetProcessID();
		float64 GetTime();

		std::string GetWorkingDirectory();

		bool RequestAccessToFile(const char* filePath); // returns whether access permission was granted

		void MessageBoxError(const char* title, const char* text);
	}
	namespace Info
	{
		uint32 GetNumberOfProcessors(); // Cores
		uint32 GetNumberOfLogicalProcessors(); // Threads

		const char* GetPlatformName();
		char* GetOSInfo();
	}

	class DynamicLibrary
	{
		/*
		platform_dl* platform_dl_load_library(const char* libraryName);
		platform_dl* platform_dl_load_library_ex(const char* libraryPath);

		void platform_dl_free_library(platform_dl* library);

		void* platform_dl_get_procedure_address(platform_dl* library, const char* functionName);

		const char* platform_dl_get_library_name_prefix(); // prefix: e.g. "" (empty string) on Windows "lib" on Linux and MacOS
		const char* platform_dl_get_library_name_suffix(); // extension name: e.g. ".dll" on Windows ".so" on Linux, ".dylib" on MacOS
		*/
	public:
		DynamicLibrary();
		DynamicLibrary(const char* libraryName);
		~DynamicLibrary();

		void Load(const char* libraryName);
		void LoadEx(const char* libraryPath);

		void* GetProcedureAddress(const char* functionName);

		constexpr const char* GetLibraryNamePrefix(); // prefix: e.g. "" (empty string) on Windows "lib" on Linux and MacOS
		constexpr const char* GetLibraryNameSuffix(); // extension name: e.g. ".dll" on Windows ".so" on Linux, ".dylib" on MacOS
	private:
		void* m_Handle;
	};
}