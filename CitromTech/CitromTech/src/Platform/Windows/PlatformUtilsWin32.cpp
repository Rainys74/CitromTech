#include "Platform/Platform.h"

#ifdef CT_PLATFORM_WINDOWS
#include "WindowsIncludes.h"
#include <process.h>

#include "CitromAssert.h"
#include "CitromMemory.h"

namespace Citrom::Platform
{
	namespace Utils
	{
		void Sleep(uint64 ms)
		{
			::Sleep(ms);
		}
		int32 GetProcessID()
		{
			return _getpid();
		}
		//float64 GetTime();

		void MessageBoxError(const char* title, const char* text)
		{
			// Flags specifying the contents and behavior of the dialog box
			UINT uType = MB_ICONERROR | MB_OK | MB_DEFBUTTON1; //| MB_ICONHAND;

			MessageBoxA(NULL, text, title, uType);
		}
	}
	namespace Info
	{
		uint32 GetNumberOfProcessors()
		{
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
			DWORD returnLength = 0;
			DWORD processorCoreCount = 0;

			// Query the required buffer size
			GetLogicalProcessorInformation(NULL, &returnLength);
			buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)Memory::Allocate(returnLength);
			CT_CORE_ASSERT(buffer, "Couldn't Allocate Buffer!");

			// Retrieve the actual information
			CT_CORE_ASSERT(GetLogicalProcessorInformation(buffer, &returnLength), "Couldn't Get Logical Processor Information!");

			DWORD byteOffset = 0;
			while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
			{
				if (buffer[byteOffset / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION)].Relationship == RelationProcessorCore)
				{
					processorCoreCount++;
				}
				byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			}

			Memory::Free(buffer);

			CT_CORE_ASSERT(processorCoreCount > 0, "Failed to retrieve the number of processor cores.");

			return processorCoreCount;
		}
		uint32 GetNumberOfLogicalProcessors()
		{
			SYSTEM_INFO sysInfo;
			GetSystemInfo(&sysInfo);

			return sysInfo.dwNumberOfProcessors;
		}

		const char* GetPlatformName()
		{
			return "Win32";
		}
		char* GetOSInfo()
		{
			OSVERSIONINFOEX osvi;
			BOOL bOsVersionInfoEx;

			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi);
			CT_CORE_ASSERT(bOsVersionInfoEx, "Couldn't Get Version!");
			
			CTL::String osInfo("Windows ");

			switch (osvi.dwMajorVersion)
			{
			case 5:
				if (osvi.dwMinorVersion == 0)
				{
					osInfo.Append("2000 ");
				}
				else if (osvi.dwMinorVersion == 1)
				{
					osInfo.Append("XP ");
				}
				else if (osvi.dwMinorVersion == 2)
				{
					osInfo.Append("Server 2003");
				}
				break;

			case 6:
				if (osvi.dwMinorVersion == 0)
				{
					osInfo.Append("Vista ");
				}
				else if (osvi.dwMinorVersion == 1)
				{
					osInfo.Append("7 ");
				}
				else if (osvi.dwMinorVersion == 2)
				{
					osInfo.Append("8 ");
				}
				else if (osvi.dwMinorVersion == 3)
				{
					osInfo.Append("8.1 ");
				}
				break;

			case 10:
				osInfo.Append("10 ");
				break;

			default:
				osInfo.Append("Unknown ");
				break;
			}

			char osNumbers[47];
			std::sprintf(osNumbers, "(NT %d.%d) Build %d", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);

			osInfo.Append(osNumbers);

			return osInfo.Data();
		}
	}
}
#endif