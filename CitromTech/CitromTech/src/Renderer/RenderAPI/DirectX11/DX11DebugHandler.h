#pragma once

#include "Core.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"
#include <dxgidebug.h>

#include "CTL/DArray.h"
#include <string>

//const char* HResultToCString(HRESULT hr);
//void DXMessageBoxErrorCStr(const char* errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function);

//#define DXCallHRCStr(x) hr = (x); if (FAILED(hr)) {const char* errorMsg = HResultToCString(hr); DXMessageBoxError(errorMsg, hr, __FILE__, __LINE__, __func__);}

std::string HResultToString(HRESULT hr);
void DXMessageBoxError(const std::string& errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function);

#define DXCallHRNoInfo(x) hr = (x); if (FAILED(hr)) {std::string errorMsg = HResultToString(hr); DXMessageBoxError(errorMsg, hr, __FILE__, __LINE__, __func__);}
#define DXCallHR(x) DXGIInfoManager::Get().ClearErrors(); DXCallHRNoInfo(x)

#define DXCall(x) DXGIInfoManager::Get().ClearErrors(); (x); if (DXGIInfoManager::Get().GetMessages().Count() > 0) {DXMessageBoxError("", 0, __FILE__, __LINE__, __func__);}

// Most of the credits for this class go to ChiliTomatoNoodle
class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager() {}

	void ClearErrors();
	CTL::DArray<std::string> GetMessages();

	static DXGIInfoManager& Get()
	{
		static DXGIInfoManager instance;
		return instance;
	}
private:
	uint64 m_Start = 0u;
	WRL::ComPtr<IDXGIInfoQueue> m_DXGIInfoQueue;
};
#endif