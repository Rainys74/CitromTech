#pragma once

#include "CTL.h"

namespace CTL::CString
{
	size_t GetLength(const char* string);

	bool IsEmptyOrNull(const char* string);
	bool IsEmpty(const char* string);

	char* Copy(char* destination, const char* source);
	char* Concatenate(char* destination, const char* source);

	const char* ReverseSearchForCharacter(const char* string, const char character);

	// compares two strings character by character. If the strings are equal, the function returns 0
	// >0 if the first non-matching character in str1 is greater (in ASCII) than that of str2.
	// <0 if the first non-matching character in str1 is lower (in ASCII) than that of str2.
	int Compare(const char* strA, const char* strB);
}
