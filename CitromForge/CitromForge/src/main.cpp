#include "Main/SharedMain.h"

#ifdef CT_PLATFORM_UNIX
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