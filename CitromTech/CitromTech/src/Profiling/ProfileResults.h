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

		using CStringHashMap = CTL::CStrHashMap<float64>; //CTL::HashMap<const char*, float64, CTL::CStringHash, CTL::CStringHashEqual>;
		using ResultOrderArray = CTL::DArray<const char*>;

		static void Submit(const char* key, const float64 time);

		static float64 RetrieveTime(const char* key);

		// Triggers a callback on every item
		static void IterateResultsCallback(CallbackFN callback);
		static void IterateResultsOrderedCallback(CallbackFN callback);

		static CStringHashMap& GetResults();
		static ResultOrderArray& GetResultOrder();

		[[deprecated("Manually retrieve the time instead.")]] static void PrintResults();
	private:
		static CStringHashMap m_Results;
		static ResultOrderArray m_Order;
	};
}