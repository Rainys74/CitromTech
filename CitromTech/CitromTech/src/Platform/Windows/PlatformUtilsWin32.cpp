#include "Platform/Platform.h"

#ifdef CT_PLATFORM_WINDOWS
#include "WindowsIncludes.h"
#include <process.h> // _getpid()

#include "CitromAssert.h"
#include "CitromMemory.h"

namespace Citrom::Platform
{
	namespace Utils
	{
		// Timing
		static float64 sg_ClockFrequency = 0;
		static LARGE_INTEGER sg_StartTime;

		void Sleep(uint64 ms)
		{
			::Sleep(ms);
		}
		int32 GetProcessID()
		{
			return _getpid();
		}
		float64 GetTime()
		{
			if (sg_ClockFrequency == 0)
			{
				// platform_get_time() initialization
				LARGE_INTEGER frequency;
				QueryPerformanceFrequency(&frequency);
				sg_ClockFrequency = 1.0 / (float64)frequency.QuadPart;
				QueryPerformanceCounter(&sg_StartTime);
				// -------------------------------
			}

			LARGE_INTEGER nowTime;
			QueryPerformanceCounter(&nowTime);
			return (float64)nowTime.QuadPart * sg_ClockFrequency;
			//return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		}

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
			CT_CORE_VERIFY(GetLogicalProcessorInformation(buffer, &returnLength), "Couldn't Get Logical Processor Information!");

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

		//static BOOL PopulateOSVersionInfo(LPOSVERSIONINFOW lpVersionInfo)
		static BOOL PopulateOSVersionInfo(OSVERSIONINFO* pOSVI)
		{
			// Ensure the passed OSVERSIONINFO structure is properly initialized
			ZeroMemory(pOSVI, sizeof(OSVERSIONINFO));
			pOSVI->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			/*// Try using RtlGetVersion first (for Windows 8 and later)
			RTL_OSVERSIONINFOEX versionInfo;
			ZeroMemory(&versionInfo, sizeof(RTL_OSVERSIONINFOEX));
			versionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEX);

			// Try to use RtlGetVersion to get OS version info
			if (RtlGetVersion(&versionInfo) == 0)
			{
				// Populate the OSVERSIONINFO structure from RtlGetVersion
				pOSVI->dwMajorVersion = versionInfo.dwMajorVersion;
				pOSVI->dwMinorVersion = versionInfo.dwMinorVersion;
				pOSVI->dwBuildNumber = versionInfo.dwBuildNumber;
				pOSVI->dwPlatformId = versionInfo.dwPlatformId;
				// Copy the CSDVersion string
				wcsncpy(pOSVI->szCSDVersion, versionInfo.szCSDVersion, sizeof(pOSVI->szCSDVersion) / sizeof(wchar_t));

				return TRUE;
			}*/

			// If RtlGetVersion failed, fallback to GetVersionEx (for older Windows versions)
			if (GetVersionEx((OSVERSIONINFO*)pOSVI))
			{
				return TRUE;
			}

			// If both methods fail, return FALSE
			return FALSE;
		}

		char* GetOSInfo()
		{
			OSVERSIONINFOEX osvi;
			BOOL bOsVersionInfoEx;

			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			//bOsVersionInfoEx = PopulateOSVersionInfo((OSVERSIONINFO*)&osvi);
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

			return osInfo.CStr();
		}
	}
}
#endif