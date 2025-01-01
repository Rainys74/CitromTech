#pragma once

#include "Core.h"

#include "CTL/HashMap.h"
#include "CTL/DArray.h"
#include "CTL/CStringHandling.h"

namespace Citrom::Profiler
{
	class ProfileResults
	{
	public:
		using CallbackFN = void (*)(const char* name, const float64 time);

		static void Submit(const char* key, const float64 time);

		static float64 RetrieveTime(const char* key);

		// Triggers a callback on every item
		static void IterateResultsCallback(CallbackFN callback);

		static CTL::HashMap<const char*, float64, CTL::CStringHash, CTL::CStringHashEqual>& GetResults();
		static CTL::DArray<const char*>& GetResultOrder();

		[[deprecated("Manually retrieve the time instead.")]] static void PrintResults();
	private:
		static CTL::HashMap<const char*, float64, CTL::CStringHash, CTL::CStringHashEqual> m_Results;
		static CTL::DArray<const char*> m_Order;
	};
}