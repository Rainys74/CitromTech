#pragma once

#include "Core.h"
#include "Logger/Logger.h"
#include "CitromAssert.h"

#include "Platform/Platform.h"

#include "CTL/HashMap.h"
#include "CTL/DArray.h"
#include "CTL/CStringHandling.h"

#include <string>

namespace Citrom::Profiler
{
	void ProfileDefaultCallback(const char* name, const float64 time);

	class ProfileResults
	{
	public:
		using CallbackFN = void (*)(const char* name, const float64 time);

		static void Submit(const char* key, const float64 time);

		static float64 RetrieveTime(const char* key);
		
		// Triggers a callback on every item
		static void IterateResultsCallback(CallbackFN callback);

		static CTL::HashMap<std::string, float64>& GetResults();
		static CTL::DArray<std::string>& GetResultOrder();

		[[deprecated("Manually retrieve the time instead.")]] static void PrintResults();
	private:
		static CTL::HashMap<std::string, float64> m_Results;
		static CTL::DArray<std::string> m_Order;
	};

	class ScopedTimer
	{
	public:
		using CallbackFN = void (*)(const char* name, const float64 time);

		ScopedTimer(const char* name, CallbackFN callback)
			: m_Name(name), m_Callback(callback)
		{
			auto nameSize = CTL::CString::GetLength(name) + 1;
			m_Name = new char[nameSize];
			Memory::Copy((void*)m_Name, name, nameSize);

			start = Platform::Utils::GetTime();
		}
		~ScopedTimer()
		{
			end = Platform::Utils::GetTime();
			duration = end - start;

			CT_CORE_ASSERT(m_Callback, "Callback function is null!");
			m_Callback(m_Name, duration);

			//CT_CORE_TRACE("Timer took {} ms", duration * 1000);
			//CT_CORE_TRACE("Timer Start {} s", start);
			//CT_CORE_TRACE("Timer End {} s", end);

			// TODO: this leads to a memory leak, but otherwise a reference system would be better
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

// TODO: profiling should be disabled on optimized builds

#define CT_PROFILE_GLOBAL_FUNCTION() auto _Intern_profileTimer ## __LINE__ = Citrom::Profiler::ScopedTimer(__func__, Citrom::Profiler::ProfileDefaultCallback)
#define CT_PROFILE_STATIC_FUNCTION(CLASS) auto _Intern_profileTimer ## __LINE__ = Citrom::Profiler::ScopedTimer(std::string(typeid(CLASS).name()).append("::").append(__func__).append("()").c_str(), Citrom::Profiler::ProfileDefaultCallback)
#define CT_PROFILE_MEMBER_FUNCTION() CT_PROFILE_STATIC_FUNCTION(*this)

#define CT_PROFILE_SCOPE(NAME) auto _Intern_profileTimer ## __LINE__ = Citrom::Profiler::ScopedTimer(NAME, Citrom::Profiler::ProfileDefaultCallback)