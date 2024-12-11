#pragma once

#include "Platform/PlatformThread.h"
#include "CTL/DArray.h"

namespace Citrom
{
	struct ThreadPoolJob
	{
		Platform::Thread::StartRoutinePFN job;
		void* args;
		bool executed;
		//bool busy;
	};

	class ThreadPool
	{
	public:
		ThreadPool(uint32 maxThreads);
		~ThreadPool();

		void Submit(void* job, void* args);
	private:
		uint32 m_MaxThreads;

		//CTL::DArray<Platform::Thread> m_WorkerThreads; // replaced because this is a strongly typed array and requires default constructors..
		Platform::Thread* m_WorkerThreads;
		Platform::Mutex m_Mutex;

		CTL::DArray<ThreadPoolJob> m_QueueJobs;

		// dyn thread worker array
		// mutex
		// maxThreads
		// job list
	};
}