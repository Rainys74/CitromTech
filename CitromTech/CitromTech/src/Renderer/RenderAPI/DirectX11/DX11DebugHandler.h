#pragma once

#include "Core.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"

const char* HResultToCString(HRESULT hr);
void DXMessageBoxError(const char* errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function);

#define DXCallHR(x) hr = (x); if (FAILED(hr)) {const char* errorMsg = HResultToCString(hr); DXMessageBoxError(errorMsg, hr, __FILE__, __LINE__, __func__);}
#endif