#pragma once

#include "Core.h"

namespace CTL::CString
{
	size_t GetLength(const char* string);

	bool IsEmptyOrNull(const char* string);
	bool IsEmpty(const char* string);

	char* Copy(char* destination, const char* source);
	char* Concatenate(char* destination, const char* source);

	char* ReverseSearchForCharacter(const char* string, const char character);
}