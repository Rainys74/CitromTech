#pragma once

#include "Core.h"

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

#define CT_PROFILE_GLOBAL_FUNCTION() auto _Intern_profileTimer ## __LINE__ = Citrom::Profiler::ScopedTimer(__func__, Citrom::Profiler::ProfileDefaultCallback)
#define CT_PROFILE_STATIC_FUNCTION(CLASS) auto _Intern_profileTimer ## __LINE__ = Citrom::Profiler::ScopedTimer(std::string(typeid(CLASS).name()).append("::").append(__func__).append("()").c_str(), Citrom::Profiler::ProfileDefaultCallback)
#define CT_PROFILE_MEMBER_FUNCTION() CT_PROFILE_STATIC_FUNCTION(*this)

#define CT_PROFILE_SCOPE(NAME) auto _Intern_profileTimer ## __LINE__ = Citrom::Profiler::ScopedTimer(NAME, Citrom::Profiler::ProfileDefaultCallback)