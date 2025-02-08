#include "Platform/Platform.h"

#ifdef CT_PLATFORM_MACOS
#include <unistd.h>
#include <sys/sysctl.h> // sysctlbyname
#include <limits.h> // PATH_MAX

#import <Cocoa/Cocoa.h>
#import <Security/Security.h>

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
		std::string GetWorkingDirectory()
		{
			char buffer[PATH_MAX];
			if (getcwd(buffer, sizeof(buffer)) != nullptr)
				return std::string(buffer);
			else
				return "";
		}

		void MessageBoxError(const char* title, const char* text)
		{
		}

		bool RequestAccessToFile(const char* filePath)
		{
			/*NSOpenPanel* panel = [NSOpenPanel openPanel];
			[panel setCanChooseFiles:YES];
			[panel setCanChooseDirectories:NO];
			
			// Optionally, set the initial file path for the user
			[panel setDirectoryURL:[NSURL fileURLWithPath:@"~/Documents"]];
			
			// This will trigger the file picker, but we don't let the user interact with it.
			if ([panel runModal] == NSModalResponseOK) 
			{
				NSURL* fileURL = [panel URL];
				return true; //return [fileURL path].UTF8String;
			}
			
			return true; //"";*/

			// Construct a URL for the file path
			NSString* nsFilePath = [NSString stringWithUTF8String:filePath];
			NSURL* fileURL = [NSURL fileURLWithPath:nsFilePath];

			// Request access to the file path using Apple’s Privacy API (only available in macOS Mojave+ (10.14))
			NSError* error = nil;
			BOOL success = [fileURL checkResourceIsReachableAndReturnError:&error];

			if (error) 
			{
				CT_CORE_FATAL("NSError: {}", error.localizedDescription.UTF8String);
				CT_CORE_ASSERT(false, "NSError!");
			}

			return success;
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
