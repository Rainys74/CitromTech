#ifdef CT_PLATFORM_WINDOWS
#include "Platform/PlatformConsole.h"

#include "Platform/Windows/WindowsIncludes.h"
#include "CTL/CStringHandling.h"

#include <string>
#include <cwchar>

#define RESET		7
#define RED			12
#define GREEN		10
#define YELLOW		14
#define BLUE		9
#define PURPLE		5

#define DARKBLUE	1
#define LIGHTBLUE	3
#define GRAY		8

namespace Citrom::Platform::Console
{
	static HANDLE GetStdStreamHandle(Stream stream)
	{
		switch (stream)
		{
		default:
			return GetStdHandle(STD_OUTPUT_HANDLE);
			break;
		case Stream::Out:
			return GetStdHandle(STD_OUTPUT_HANDLE);
			break;
		case Stream::Error:
			return GetStdHandle(STD_ERROR_HANDLE);
			break;
		}
	}

	void SetTextColor(TextColor textColor)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (textColor)
		{
			default:
				SetConsoleTextAttribute(hConsole, RESET);
				break;
			case TextColor::Reset:
				SetConsoleTextAttribute(hConsole, RESET);
				break;
			case TextColor::Red:
				SetConsoleTextAttribute(hConsole, RED);
				break;
			case TextColor::Green:
				SetConsoleTextAttribute(hConsole, GREEN);
				break;
			case TextColor::Yellow:
				SetConsoleTextAttribute(hConsole, YELLOW);
				break;
			case TextColor::Blue:
				SetConsoleTextAttribute(hConsole, BLUE);
				break;
			case TextColor::Purple:
				SetConsoleTextAttribute(hConsole, PURPLE);
				break;
		}
	}
	void PrintText(const char* text, Stream stdStream)
	{
		/*HANDLE hConsole;
		switch (stdStream)
		{
			default:
				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				break;
			case Stream::Out:
				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				break;
			case Stream::Error:
				hConsole = GetStdHandle(STD_ERROR_HANDLE);
				break;
		}*/

		HANDLE hConsole = GetStdStreamHandle(stdStream);

		DWORD dwCharsWritten;
		WriteConsoleA(hConsole, text, CTL::CString::GetLength(text), &dwCharsWritten, NULL);
	}
	void PrintText(const char8_t* text, Stream stdStream)
	{
		HANDLE hConsole = GetStdStreamHandle(stdStream);

		DWORD dwCharsWritten;
		WriteConsoleA(hConsole, text, std::char_traits<char8_t>::length(text), &dwCharsWritten, NULL);
	}
	void PrintText(const char16_t* text, Stream stdStream)
	{
		HANDLE hConsole = GetStdStreamHandle(stdStream);

		/*size_t length = 0;
		while (text[length] != u'\0') 
		{
			++length;
		}*/

		DWORD dwCharsWritten;
		WriteConsoleW(hConsole, text, std::char_traits<char16_t>::length(text), &dwCharsWritten, NULL);
	}
	void PrintText(const char32_t* text, Stream stdStream)
	{
	}
	void PrintText(const wchar_t* text, Stream stdStream)
	{
		HANDLE hConsole = GetStdStreamHandle(stdStream);

		DWORD dwCharsWritten;
		WriteConsoleW(hConsole, text, wcslen(text), &dwCharsWritten, NULL);
	}
}

#endif