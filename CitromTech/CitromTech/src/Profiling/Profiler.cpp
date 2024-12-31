#include "Profiler.h"

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
}