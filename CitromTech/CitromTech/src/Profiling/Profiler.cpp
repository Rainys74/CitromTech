#include "Profiler.h"
#include "ProfileResults.h"

#include "Platform/Platform.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"

namespace Citrom::Profiler
{
	CTL::HashMap<const char*, float64, CTL::CStringHash, CTL::CStringHashEqual> ProfileResults::m_Results;
	CTL::DArray<const char*> ProfileResults::m_Order;

	void ProfileDefaultCallback(const char* name, const float64 time)
	{
		ProfileResults::Submit(name, time);
	}

	void ProfileResults::Submit(const char* key, const float64 time)
	{
		// Check if the key is new
		if (m_Results.find(key) == m_Results.end())
		{
			m_Order.PushBack(key);
		}

		//for (uint32 i = m_Order.Count() - 1; i >= 0; --i);

		m_Results[key] = time;
	}

	float64 ProfileResults::RetrieveTime(const char* key)
	{
		return m_Results[key];
	}
	void ProfileResults::IterateResultsCallback(CallbackFN callback)
	{
		CT_CORE_ASSERT(callback, "Null callback was passed!");
		for (const auto& result : m_Results)
		{
			callback(result.first, result.second);
		}
	}
	void ProfileResults::IterateResultsOrderedCallback(CallbackFN callback)
	{
		for (const char* key : m_Order)
		{
			callback(key, m_Results[key]);
		}
	}
	CTL::HashMap<const char*, float64, CTL::CStringHash, CTL::CStringHashEqual>& ProfileResults::GetResults()
	{
		return m_Results;
	}
	CTL::DArray<const char*>& ProfileResults::GetResultOrder()
	{
		return m_Order;
	}
	void ProfileResults::PrintResults()
	{
		for (const auto& result : m_Results)
		{
			CT_CORE_TRACE("Profiling {} took {} ms", result.first, result.second * 1000);
		}
	}

	ScopedTimer::ScopedTimer(const char* name, CallbackFN callback)
		: m_Name(name), m_Callback(callback)
	{
		auto nameSize = CTL::CString::GetLength(name) + 1;
		m_Name = new char[nameSize];
		Memory::Copy((void*)m_Name, name, nameSize);

		start = Platform::Utils::GetTime();
	}
	ScopedTimer::~ScopedTimer()
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
}