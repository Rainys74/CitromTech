#ifdef CT_PLATFORM_UNIX
#include "Platform/PlatformConsole.h"

#include <cstdio>
#include <cwchar>

#include <unistd.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m" //"\e[35m"

namespace Citrom::Platform::Console
{
	void CreateConsole() {}
	void FreeConsole() {}

    static std::FILE* GetStdStreamHandle(Stream stream)
	{
		switch (stream)
		{
		default:
			return stdout;
			break;
		case Stream::Out:
			return stdout;
			break;
		case Stream::Error:
			return stderr;
			break;
		}
	}

    void SetTextColor(TextColor textColor)
    {
        switch (textColor)
		{
			default:
                std::printf(RESET);
				break;
			case TextColor::Reset:
				std::printf(RESET);
				break;
			case TextColor::Red:
				std::printf(RED);
				break;
			case TextColor::Green:
				std::printf(GREEN);
				break;
			case TextColor::Yellow:
				std::printf(YELLOW);
				break;
			case TextColor::Blue:
				std::printf(BLUE);
				break;
			case TextColor::Purple:
				std::printf(PURPLE);
				break;
		}
    }
	void PrintText(const CTL::String& text, Stream stdStream)
	{
		int fd = 0;
		switch (stdStream)
		{
		default:
		case Stream::Out:
			fd = STDOUT_FILENO;
			break;
		case Stream::Error:
			fd = STDERR_FILENO;
			break;
		}
		
		(void)write(fd, text.Data(), text.Length());
	}
	void PrintText(const char* text, Stream stdStream)
    {
        std::fprintf(GetStdStreamHandle(stdStream), text);
    }		
	void PrintText(const char8_t* text, Stream stdStream)
    {
        std::fprintf(GetStdStreamHandle(stdStream), reinterpret_cast<const char*>(text));
    }	
	void PrintText(const char16_t* text, Stream stdStream)
    {
        
    }
	void PrintText(const char32_t* text, Stream stdStream)
    {

    }
	void PrintText(const wchar_t* text, Stream stdStream)
    {
        // TODO: none of these work for MacOS
        std::fwprintf(GetStdStreamHandle(stdStream), text);
        //std::fwprintf(GetStdStreamHandle(stdStream), L"%ls", text);
        //std::wprintf(L"%ls", text);
    }
}

#endif
