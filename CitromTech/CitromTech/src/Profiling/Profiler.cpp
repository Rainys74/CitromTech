#include "Profiler.h"

namespace Citrom::Profiler
{
	CTL::HashMap<const char*, float64> ProfileResults::m_Results;

	void ProfileDefaultCallback(const char* name, const float64 time)
	{
		ProfileResults::Submit(name, time);
	}

	void ProfileResults::Submit(const char* key, const float64 time)
	{
		m_Results[key] += time;
	}

	float64 ProfileResults::RetrieveTime(const char* key)
	{
		return m_Results[key];
	}
	void ProfileResults::PrintResults()
	{
		for (const auto& result : m_Results)
		{
			//CT_CORE_TRACE("Profiling {} took {} ms", result.first, result.second * 1000);
			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Trace, "{}(): " "Profiling {} took {} ms", __func__, result.first, result.second * 1000);
		}
	}
}