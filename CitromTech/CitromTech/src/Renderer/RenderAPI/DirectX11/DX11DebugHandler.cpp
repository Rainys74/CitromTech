#include "DX11DebugHandler.h"

#ifdef CT_PLATFORM_WINDOWS
#include <sstream>
#include <bitset>

#include "CTL/ScopedPointer.h"
#include "CitromAssert.h"

/*const char* HResultToCString(HRESULT hr)
{
	if (HRESULT_FACILITY(hr) == FACILITY_WIN32)
	{
		// If the error is from Win32, we need to strip the facility portion
		hr = HRESULT_CODE(hr);
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr
	);

	std::string message;
	if (size)
	{
		message = messageBuffer;
		LocalFree(messageBuffer); // Free the buffer allocated by FormatMessage
	}
	else
	{
		std::stringstream ss;
		ss << "Unknown error code: 0x" << std::hex << hr;
		message = ss.str();
	}

	return message.c_str();
}

void DXMessageBoxErrorCStr(const char* errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function)
{
	std::stringstream ss;
	ss << "Error Code: 0x" << std::hex << hr << " = " << std::dec << hr << " = 0o" << std::oct << hr << " = 0b" << std::bitset<32>(hr);
	ss << '\n' << '\n';
	ss << "[Error Message]:\n";
	ss << errorMsg;
	ss << '\n';
	ss << "[In]:\n";
	ss << file << "(" << line << "): " << function << "();";

	MessageBoxA(nullptr, ss.str().c_str(), "DXCallHR Failed", MB_ICONERROR);
}*/

std::string HResultToString(HRESULT hr)
{
	if (HRESULT_FACILITY(hr) == FACILITY_WIN32)
	{
		// If the error is from Win32, we need to strip the facility portion
		hr = HRESULT_CODE(hr);
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr
	);

	std::string message;
	if (size)
	{
		message = messageBuffer;
		LocalFree(messageBuffer); // Free the buffer allocated by FormatMessage
	}
	else
	{
		std::stringstream ss;
		ss << "Unknown error code: 0x" << std::hex << hr;
		message = ss.str();
	}

	return message;
}

void DXMessageBoxError(const std::string& errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function)
{
	std::stringstream ss;
	ss << "Error Code: 0x" << std::hex << hr << " = " << std::dec << (unsigned long)hr << " = 0o" << std::oct << hr << " = 0b" << std::bitset<32>(hr); //<< " = " << DXGetErrorString();
	ss << '\n' << '\n';
	ss << "[Error Message]:\n";
	ss << errorMsg.c_str();
	ss << '\n';

	ss << "[DXGI Debug Info]:\n";
	for (auto& infoMessage : DXGIInfoManager::Get().GetMessages())
	{
		ss << infoMessage << '\n';
	}
	ss << '\n';

	ss << "[In]:\n";
	ss << file << "(" << line << "): " << function << "();";

	MessageBoxA(nullptr, ss.str().c_str(), "DXCallHR Failed", MB_ICONERROR);
}

#pragma comment(lib, "dxguid.lib")

DXGIInfoManager::DXGIInfoManager()
{
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	CT_CORE_ASSERT(hModDxgiDebug != nullptr, "Failed to load DXGIDebug.dll!");

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	CT_CORE_ASSERT(DxgiGetDebugInterface != nullptr, "Failed to load function DxgiGetDebugInterface!");

	HRESULT hr;
	DXCallHRNoInfo(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_DXGIInfoQueue));

	//FreeLibrary(hModDxgiDebug);
}

void DXGIInfoManager::ClearErrors()
{
	m_Start = m_DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

CTL::DArray<std::string> DXGIInfoManager::GetMessages()
{
	CTL::DArray<std::string> messages;
	const auto end = m_DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = m_Start; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		// get the size of message i in bytes
		DXCallHR(m_DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		// get the message and push its description into the vector
		DXCallHR(m_DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.PushBack(pMessage->pDescription);
	}
	return messages;
}
#endif