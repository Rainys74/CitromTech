#pragma once

#include "Platform/PlatformThread.h"
#include "CTL/DArray.h"

namespace Citrom
{
	class ThreadPool
	{
	public:
		ThreadPool(const uint32 maxThreads);
		~ThreadPool();

		void Submit(void* job, void* args);
	private:
		uint32 m_MaxThreads;

		CTL::DArray<Platform::Thread> m_Threads; // workers

		// dyn thread worker array
		// mutex
		// maxThreads
		// job list
	};
}