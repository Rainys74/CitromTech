#pragma once

#ifdef CT_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsIncludes.h"
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#include <wrl.h>
using namespace Microsoft;

#define SAFE_RELEASE(x) if (x != nullptr) { x->Release(); x = nullptr; }
#endif