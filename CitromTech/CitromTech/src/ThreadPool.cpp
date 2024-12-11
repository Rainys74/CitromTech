#include "ThreadPool.h"
#include "Logger/Logger.h"
#include "CitromMemory.h"

namespace Citrom
{
	using namespace Platform;
	static void ThreadPoolWork(ThreadPool* pool)
	{
		CT_VERBOSE("Thread ID: ({}); Thread Pool Working!", Thread::GetCurrentID());
	}

	ThreadPool::ThreadPool(uint32 maxThreads)
		: m_MaxThreads(maxThreads)
	{
		m_WorkerThreads = (Thread*)Memory::Allocate(maxThreads * sizeof(Thread));
		for (uint32 i = 0; i < maxThreads; i++)
		{
			//m_WorkerThreads.PushBack(*new Thread(reinterpret_cast<Thread::StartRoutinePFN>(ThreadPoolWork), this));
		}
	}

	ThreadPool::~ThreadPool()
	{
		for (uint32 i = 0; i < m_MaxThreads; i++)
		{
			m_WorkerThreads[i].Join();
		}
	}

	void ThreadPool::Submit(void* job, void* args)
	{
		ThreadPoolJob poolJob;
		poolJob.job = (Thread::StartRoutinePFN)job;
		poolJob.args = args;
		poolJob.executed = false;

		m_Mutex.Lock();
		m_QueueJobs.PushBack(poolJob);
		m_Mutex.Unlock();
	}
}