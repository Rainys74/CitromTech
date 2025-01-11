#pragma once

#include "Core.h"

#include <string>

/*namespace CTL
{
	// Forward Declarations for CTL
	template<typename K, typename T, typename H, typename C>
	class HashMap;

	template<typename T>
	class DArray;

	class CStringHash;
	class CStringHashEqual;
}*/

namespace Citrom::Profiler
{
	void ProfileDefaultCallback(const char* name, const float64 time);

	class ScopedTimer
	{
	public:
		using CallbackFN = void (*)(const char* name, const float64 time);

		ScopedTimer(const char* name, CallbackFN callback);
		~ScopedTimer();
	public:
		float64 start = 0, end = 0;
		float64 duration = 0;
	private:
		const char* m_Name;
		CallbackFN m_Callback;
	};
}

// TODO: profiling should be disabled on optimized builds
#define _INTERN_CT_PROFILE_MACRO(NAMECSTR) auto _Intern_profileTimer ## __LINE__ = Citrom::Profiler::ScopedTimer(NAMECSTR, Citrom::Profiler::ProfileDefaultCallback)

#define CT_PROFILE_GLOBAL_FUNCTION() _INTERN_CT_PROFILE_MACRO(__func__)
// The line: .substr(std::string(typeid(CLASS).name()).find("class ") == 0 ? 6 : (std::string(typeid(CLASS).name()).find("struct ") == 0 ? 7 : 0)) trims the prefixes to make the profiler's function names shorter
// Original macro: #define CT_PROFILE_STATIC_FUNCTION(CLASS) _INTERN_CT_PROFILE_MACRO(std::string(typeid(CLASS).name()).append("::").append(__func__).append("()").c_str())
#define CT_PROFILE_STATIC_FUNCTION(CLASS) _INTERN_CT_PROFILE_MACRO(std::string(typeid(CLASS).name()).substr(std::string(typeid(CLASS).name()).find("class ") == 0 ? 6 : (std::string(typeid(CLASS).name()).find("struct ") == 0 ? 7 : 0)).append("::").append(__func__).append("()").c_str())
#define CT_PROFILE_MEMBER_FUNCTION() CT_PROFILE_STATIC_FUNCTION(*this) //_INTERN_CT_PROFILE_MACRO(CT_PRETTY_FUNCTION)

#define CT_PROFILE_SCOPE(NAME) _INTERN_CT_PROFILE_MACRO(NAME)