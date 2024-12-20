#pragma once

#include "Core.h"
#include "Logger/Logger.h"
#include "CitromAssert.h"

#include "Platform/Platform.h"

#include "CTL/HashMap.h"

namespace Citrom::Profiler
{
	void ProfileDefaultCallback(const char* name, const float64 time);

	class ProfileResults
	{
	public:
		static void Submit(const char* key, const float64 time);

		static float64 RetrieveTime(const char* key);

		// Prefer to manually retrieve the time
		static void PrintResults();
	private:
		static CTL::HashMap<const char*, float64> m_Results;
	};

	class ScopedTimer
	{
	public:
		using CallbackFN = void (*)(const char* name, const float64 time);

		ScopedTimer(const char* name, CallbackFN callback)
			: m_Name(name), m_Callback(callback)
		{
			//auto nameSize = CTL::CString::GetLength(name) + 1;
			//m_Name = new char[nameSize];
			//Memory::Copy((void*)m_Name, name, nameSize);

			start = Platform::Utils::GetTime();
		}
		~ScopedTimer()
		{
			end = Platform::Utils::GetTime();
			duration = end - start;

			CT_CORE_ASSERT(m_Callback, "Callback function is null!");
			m_Callback(m_Name, duration);

			//CT_CORE_TRACE("Timer took {} ms", duration * 1000);

			//delete m_Name;
		}
	public:
		float64 start = 0, end = 0;
		float64 duration = 0;
	private:
		const char* m_Name;
		CallbackFN m_Callback;
	};
}

#define PROFILE_GLOBAL_FUNCTION() Citrom::Profiler::ScopedTimer(__func__, Citrom::Profiler::ProfileDefaultCallback)
//#define PROFILE_STATIC_FUNCTION(CLASS) Citrom::Profiler::ScopedTimer(std::string(typeid(CLASS).name()).append("::").append(__func__).append("()").c_str(), Citrom::Profiler::ProfileDefaultCallback)
#define PROFILE_STATIC_FUNCTION(CLASS) Citrom::Profiler::ScopedTimer(std::string(typeid(CLASS).name()).append("::").append(__func__).append("()").c_str(), Citrom::Profiler::ProfileDefaultCallback)
//#define PROFILE_MEMBER_FUNCTION() Citrom::Profiler::ScopedTimer(std::string(typeid(*this).name()).append("::").append(__func__).append("()").c_str(), Citrom::Profiler::ProfileDefaultCallback)
#define PROFILE_MEMBER_FUNCTION() PROFILE_STATIC_FUNCTION(*this)

#define PROFILE_SCOPE(NAME) Citrom::Profiler::ScopedTimer(NAME, Citrom::Profiler::ProfileDefaultCallback)