#pragma once

constexpr const char* GetBuildType()
{
#if   defined(CT_DEBUG)
	return "Debug";
#elif defined(CT_RELEASE)
	return "Release";
#elif defined(CT_OPTIMIZATION)
	return "Optimization";
#else
	return nullptr;
#endif
}

//#include <type_traits>
#include <utility>

template <typename T, typename U>
constexpr T CCast(U&& value)
{
	return (T)(std::forward<U>(value));
}

template <typename T, typename U>
constexpr T SCast(U&& value) 
{
	return static_cast<T>(std::forward<U>(value));
}
template <typename T, typename U>
constexpr T RCast(U&& value)
{
	return reinterpret_cast<T>(std::forward<U>(value));
}