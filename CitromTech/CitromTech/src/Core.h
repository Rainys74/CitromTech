#pragma once

#ifdef _MSC_VER // && CT_PLATFORM_WINDOWS
#define DLL_EXPORT __declspec(dllexport) 
#define DLL_IMPORT __declspec(dllimport) 
#elif defined(__GNUC__) || defined(__clang__)
#define DLL_EXPORT __attribute__((visibility("default"))) // __attribute__((dllexport))
#define DLL_IMPORT __attribute__((visibility("default"))) // __attribute__((dllimport))
#endif

#ifdef CT_BUILD_LIB_AS_DLL
	#ifdef CT_BUILD_LIB
		#define CITROM_API DLL_EXPORT
	#else
		#define CITROM_API DLL_IMPORT
	#endif
#elif defined(CT_BUILD_LIB_AS_STATIC)
	#define CITROM_API
#else
#error "You must define in the application and library for Citrom Tech to either be built as a Static Library, Or a Dynamic Library!"
#endif

#ifndef CT_NO_TYPEDEFS
	#if defined(_MSC_VER) // Microsoft Visual Studio
	#define FORCE_INLINE __forceinline
	#elif defined(__GNUC__) || defined(__clang__) // GCC and Clang
	#define FORCE_INLINE inline __attribute__((always_inline))
	#else
	#define FORCE_INLINE inline
	#endif

	using int8 = signed char;
	using int16 = short;
	using int32 = int;
	using int64 = long long;

	using uint = unsigned int;
	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;

	using float32 = float;
	using float64 = double;
	using floatl = long double;

	#include <cstdint>
	using int8fast = std::int_fast8_t;
	using int16fast = std::int_fast16_t;
	using int32fast = std::int_fast32_t;
	using int64fast = std::int_fast64_t;

	using uint8fast = std::uint_fast8_t;
	using uint16fast = std::uint_fast16_t;
	using uint32fast = std::uint_fast32_t;
	using uint64fast = std::uint_fast64_t;

	#include <cstddef> // size_t, wchar_t etc.
#endif

// TODO: figure out if this is how you wanna do things
#ifdef __unix__
#define CT_PLATFORM_UNIX
#endif

// ------------MACROS-----------------------

#if defined(CT_PLATFORM_WINDOWS) && defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK()
#endif

#define BIT(x) (1 << x)
// -----------------------------------------