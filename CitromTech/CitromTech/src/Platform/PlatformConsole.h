#pragma once

namespace Citrom::Platform::Console
{
	enum class TextColor
	{
		Reset,
		Red,
		Green,
		Yellow,
		Blue,
		Purple
	};
	enum class Stream
	{
		Out,	// stdout
		Error,	// stderr
		In		// stdin
	};

	void SetTextColor(TextColor textColor);
	void PrintText(const char* text, Stream stdStream);		// ANSI/UTF-8
	void PrintText(const char8_t* text, Stream stdStream);	// UTF-8
	void PrintText(const char16_t* text, Stream stdStream); // UTF-16
	void PrintText(const char32_t* text, Stream stdStream); // UTF-32
	void PrintText(const wchar_t* text, Stream stdStream);	// Wide (Compiler Dependent), theoretically should be UTF-16 for Windows, UTF-32 for Linux
	//void Log(TextColor textColor, const char* text);
}