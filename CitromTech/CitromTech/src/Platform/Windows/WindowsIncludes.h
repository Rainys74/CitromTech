#pragma once

#ifdef CT_PLATFORM_WINDOWS
//#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc); // implemented in CommandLineToArgvA.cpp
#endif