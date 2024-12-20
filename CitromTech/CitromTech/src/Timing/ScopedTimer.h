#pragma once

#include "Core.h"

// Uncomment if needed or completely remove this dual functionality
//#define SCOPED_TIMER_USE_PLATFORM_SPECIFIC_BACKEND

#ifndef SCOPED_TIMER_USE_PLATFORM_SPECIFIC_BACKEND
#include <chrono>
#else
#include "Platform/Platform.h"
#endif

#include "Logger/Logger.h"

namespace Citrom
{
#ifdef SCOPED_TIMER_USE_PLATFORM_SPECIFIC_BACKEND
	class ScopedTimer
	{
	public:
		ScopedTimer()
		{
			start = Platform::Utils::GetTime();
		}
		~ScopedTimer()
		{
			end = Platform::Utils::GetTime();
			duration = end - start;

			CT_CORE_TRACE("Timer took {} ms", duration * 1000);
		}
	public:
		float64 start = 0, end = 0;
		float64 duration = 0;
	};
#else
	class ScopedTimer
	{
	public:
		ScopedTimer()
		{
			start = std::chrono::high_resolution_clock::now();
		}
		~ScopedTimer()
		{
			end = std::chrono::high_resolution_clock::now();
			duration = end - start;

			CT_CORE_TRACE("Timer took {} ms", duration.count() * 1000.0f);
		}
	public:
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<float> duration;
	};
#endif
}