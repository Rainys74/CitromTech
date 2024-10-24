#include "Main/SharedMain.h"

#if defined(CT_PLATFORM_WINDOWS) && !defined(CT_BUILD_APP_AS_CONSOLE)
#include "Platform/Windows/WindowsIncludes.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	/*
	//auto cmdLine = GetCommandLineW();
	auto cmdLine = pCmdLine;

	int argc;
	auto wideArgV = CommandLineToArgvW(cmdLine, &argc);
	*/

	int argc;
	auto argv = CommandLineToArgvA(GetCommandLineA(), &argc);

	return SharedMain(argc, argv);
}
#else
int main(int argc, char* argv[])
{
	return SharedMain(argc, argv);
}
#endif