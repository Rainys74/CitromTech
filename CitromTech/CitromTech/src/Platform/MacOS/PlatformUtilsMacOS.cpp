#include "Platform/Platform.h"

//#ifdef CT_PLATFORM_MACOS
#if 1
#include <unistd.h>
#include <sys/sysctl.h> // sysctlbyname

#include "CitromAssert.h"
#include "CitromMemory.h"

#include "CTL/CStringHandling.h"

namespace Citrom::Platform
{
	namespace Utils
	{
		void Sleep(uint64 ms)
		{
            usleep(ms * 1000); // expects time in microseconds
		}
		int32 GetProcessID()
		{
			return getpid();
		}
		float64 GetTime()
		{
            /* ChatGPT Yap:
            if (sg_ClockFrequency == 0) {
                mach_timebase_info_data_t timebase;
                mach_timebase_info(&timebase);
                sg_ClockFrequency = (double)timebase.numer / timebase.denom / 1e9;
                sg_StartTime = mach_absolute_time();
            }

            uint64_t nowTime = mach_absolute_time();
            return (double)(nowTime - sg_StartTime) * sg_ClockFrequency;
            */

			return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		}

		void MessageBoxError(const char* title, const char* text)
		{
		}
	}
	namespace Info
	{
		uint32 GetNumberOfProcessors()
		{
			int count;
            size_t size = sizeof(count);

            sysctlbyname("hw.physicalcpu", &count, &size, nullptr, 0);

            CT_CORE_ASSERT(count > 0, "Failed to retrieve the number of processor cores.");

            return count;
		}
		uint32 GetNumberOfLogicalProcessors()
		{
			return (uint32)sysconf(_SC_NPROCESSORS_ONLN); // or get sysctl hw.logicalcpu
		}

		const char* GetPlatformName()
		{
			return "MacOS"; // Cocoa?
		}
		char* GetOSInfo()
		{
			static char osInfo[128];
            size_t size = sizeof(osInfo);

            if (sysctlbyname("kern.osrelease", osInfo, &size, nullptr, 0) == 0) 
            {
                char version[32];
                size_t versionSize = sizeof(version);

                CT_CORE_VERIFY(sysctlbyname("kern.osversion", version, &versionSize, nullptr, 0) == 0, "Failed to retrieve sysctl 'kern.osversion'");

                char kernelType[32];
                size_t kernelTypeSize = sizeof(kernelType);

                CT_CORE_VERIFY(sysctlbyname("kern.ostype", kernelType, &kernelTypeSize, nullptr, 0) == 0, "Failed to retrieve sysctl 'kern.ostype'");

                std::snprintf(osInfo, sizeof(osInfo), "macOS (Kernel: %s, Build: %s)", /*osInfo,*/ kernelType, version); // e.g. macOS (Kernel: Darwin, Build: 24A348)
            }
            else 
            {
                CTL::CString::Copy(osInfo, "Unknown macOS Version");
            }

            return osInfo;
		}
	}
}
#endif