#include "DX11DebugHandler.h"

#include <string>
#include <sstream>
#include <bitset>

const char* HResultToCString(HRESULT hr)
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

void DXMessageBoxError(const char* errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function)
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
}