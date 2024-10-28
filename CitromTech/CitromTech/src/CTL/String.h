#pragma once

#include "CTL.h"

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
	// TODO: Perhaps implement a template system that allows to implement
	// different types of encoding for strings easily, without duplicating code: 
	// ANSI, UTF-8, UTF-16, UTF-32, Wide etc.
	class String 
	{
	public:
		String();
		String(const char* initialString);
		~String();

		void Append(const char* data);
		void PushBack(const char character);
		void Join(const String& secondString);

		char* CStr() const; // ANSI/UTF-8
		wchar_t* WStr(); // Compiler dependent, most likely UTF-16 on Windows, UTF-32 on Linux.

		//char16_t* UTF16Str(); // UTF-16
		//char32_t* UTF32Str(); // UTF-32
	private:
		char* m_Data = nullptr;
		size_t m_Length = 0;
	};
}