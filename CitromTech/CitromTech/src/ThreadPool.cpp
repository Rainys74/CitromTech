#include "ThreadPool.h"
#include "Logger/Logger.h"
#include "Profiling/Profiler.h"

#include "Platform/Platform.h"

namespace Citrom
{
	ThreadPool* ThreadPool::s_Instance;

	using namespace Platform;
	static void ThreadPoolWork(ThreadPool* threadPool)
	{
		CT_VERBOSE("Thread ID: ({}); Thread Pool Working!", Thread::GetCurrentID());
		while (true)
		{
			//threadPool->GetMutex().Lock();
			while (threadPool->GetJobCount() == 0)
			{
				//threadPool->GetMutex().Unlock();

				// probably look into replacing with Thread's sleep function
				Platform::Utils::Sleep(1); // Sleep for 1 up to 10 ms to reduce CPU usage

				//threadPool->GetCondition().Wait(threadPool->GetMutex());
			}

			//thread_mutex_lock(callbackData->mutex);
			//callbackData->jobs->count--;
			//thread_pool_job currentJob = callbackData->jobs->data[callbackData->jobs->count];
			//thread_mutex_unlock(callbackData->mutex);
			//
			//currentJob.job(currentJob.args);

			threadPool->GetMutex().Lock();

			if (threadPool->GetJobCount() > 0)
			{
				//callbackData->jobs->count--;
				//thread_pool_job currentJob = callbackData->jobs->data[callbackData->jobs->count];

				ThreadPoolJob currentJob = (*threadPool->GetJobQueue())[threadPool->GetJobCount() - 1];
				threadPool->GetJobQueue()->PopBack();

				threadPool->GetMutex().Unlock();

				currentJob.job(currentJob.args);

				threadPool->GetAtomicFinishedLabel().FetchAdd(1);
			}
			else
			{
				//threadPool->GetCondition().Wait(threadPool->GetMutex());
				threadPool->GetMutex().Unlock();
			}
		}
	}

	ThreadPool::ThreadPool(uint32 maxThreads)
		: m_MaxThreads(maxThreads)
	{
		m_FinishedLabel.Store(0);
		for (uint32 i = 0; i < maxThreads; i++)
		{
			m_WorkerThreads.PushBack(*new Thread(reinterpret_cast<Thread::StartRoutinePFN>(ThreadPoolWork), this));
		}
	}

	ThreadPool::~ThreadPool()
	{
		m_Condition.NotifyAll();

		for (Thread& worker : m_WorkerThreads) 
		{
			// TODO: figure this out lol
			//worker.Join();
			worker.TryCancel();
		}
	}

	void ThreadPool::Submit(void* job, void* args)
	{
		CT_PROFILE_MEMBER_FUNCTION();

		m_CurrentLabel++;

		ThreadPoolJob poolJob;
		poolJob.job = reinterpret_cast<Thread::StartRoutinePFN>(job);
		poolJob.args = args;
		poolJob.executed = false;

		m_Mutex.Lock();
		m_QueueJobs.PushBack(poolJob);
		m_Mutex.Unlock();

		m_Condition.NotifyOne();
	}
}