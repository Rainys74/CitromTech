#pragma once

#include "CTL.h"
#include "DArray.h"

// Prefer to use std::string instead of CTL::String. Reasons:
// My string lacks a lot of functionality and the functionality it does have,
// is underwhelming to say the least, instead of extending my string right now mid engine production
// i'd rather switch to a string that has been worked on by better brains than me,
// and which has better support, has been tested more thoroughly and has far better functionality
#include <string>

namespace CTL
{
	namespace StdString
	{
		CTL::DArray<std::string> Split(const std::string& string, const char delimiter);
	}
}

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
	template<bool bAllocateNewString = false>
	class ScopedCStr
	{
	public:
		ScopedCStr()
			: m_String(nullptr) {}
		ScopedCStr(const char* string)
		{
			if constexpr (bAllocateNewString)
			{
				const auto strLength = strlen(string) + 1;
				m_String = new char[strLength];
				memcpy(m_String, string, strLength);
			}
			else
			{
				m_String = const_cast<char*>(string);
			}
		}
		~ScopedCStr()
		{
			delete m_String;
		}

		operator const char* () const { return m_String; }
		operator char* () { return m_String; }
	private:
		char* m_String;
	};

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

		ScopedCStr<false> CStr() const; // ANSI/UTF-8
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

#include <format>

template <>
struct std::formatter<CTL::ScopedCStr<false>> : std::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const CTL::ScopedCStr<false>& p, FormatContext& ctx)
	{
		return std::format_to(ctx.out(), "{}", (const char*)p);
	}
};