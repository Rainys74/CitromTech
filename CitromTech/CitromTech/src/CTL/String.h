#pragma once

#include "CTL.h"

// Prefer to use std::string instead of CTL::String. Reasons:
// My string lacks a lot of functionality and the functionality it does have,
// is underwhelming to say the least, instead of extending my string right now mid engine production
// i'd rather switch to a string that has been worked on by better brains than me,
// and which has better support, has been tested more thoroughly and has far better functionality
#include <string>

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
	// TODO: Perhaps implement a template system that allows to implement
	// different types of encoding for strings easily, without duplicating code: 
	// ANSI, UTF-8, UTF-16, UTF-32, Wide etc.
	class /*[[deprecated("Use std::string instead.")]]*/ String
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

		void CStrFree(char* cStr){}

		// TODO: Find, RFind (Reverse Find), SubStr

		FORCE_INLINE char* Data() const { return m_Data; } // returns the underlying data
		FORCE_INLINE size_t Length() const { return m_Length; } // returns the character count
	private:
		char* m_Data = nullptr;
		size_t m_Length = 0;
	};

	/*template<typename T, typename Liberator>
	class TCStrGuard
	{
	public:
		TCStrGuard(T cStr)
			: m_CStr(cStr) {}
		~TCStrGuard()
		{
			Liberator(m_CStr);
		}

	private:
		T m_CStr;
	};

	class StringInternalCStrLiberator
	{
	public:
		void operator()(const char* cStr)
		{
			string.CStrFree(string);
		}
	};

	using CStrGuard = TCStrGuard<const String&, >*/
}