#include "Profiler.h"

namespace Citrom::Profiler
{
	CTL::HashMap<std::string, float64> ProfileResults::m_Results;
	CTL::DArray<std::string> ProfileResults::m_Order;

	void ProfileDefaultCallback(const char* name, const float64 time)
	{
		ProfileResults::Submit(name, time);
	}

	void ProfileResults::Submit(const char* key, const float64 time)
	{
		std::string keyStr(key);

		// Check if the key is new
		if (m_Results.find(keyStr) == m_Results.end())
		{
			m_Order.PushBack(keyStr);
		}

		m_Results[keyStr] = time;
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
			callback(result.first.c_str(), result.second);
		}
	}
	CTL::HashMap<std::string, float64>& ProfileResults::GetResults()
	{
		return m_Results;
	}
	CTL::DArray<std::string>& ProfileResults::GetResultOrder()
	{
		return m_Order;
	}
	void ProfileResults::PrintResults()
	{
		for (const auto& result : m_Results)
		{
			CT_CORE_TRACE("Profiling {} took {} ms", result.first.c_str(), result.second * 1000);
		}
	}
}