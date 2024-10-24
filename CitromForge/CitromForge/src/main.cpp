#include "Main/SharedMain.h"

#if defined(CT_PLATFORM_MACOS) || defined(CT_PLATFORM_LINUX) || defined(CT_PLATFORM_UNIX)
int main(int argc, char* argv[])
{
	return SharedMain(argc, argv);
}
#elif defined(CT_PLATFORM_WINDOWS)
#include "Platform/Windows/WindowsIncludes.h"

int main(int argc, char* argv[])
{
	return SharedMain(argc, argv);
}
#endif