#include "ThreadPool.h"
#include "Logger/Logger.h"

namespace Citrom
{
	using namespace Platform;
	static void ThreadPoolWork(ThreadPool* threadPool)
	{
		CT_VERBOSE("Thread ID: ({}); Thread Pool Working!", Thread::GetCurrentID());
		while (true)
		{
			while (threadPool->GetJobCount() == 0)
			{
				//thread_sleep(&callbackData->threads->data[callbackData->threadID], 1000); // Sleep for 1 ms to reduce CPU usage
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
				threadPool->GetJobQueue()->PopBack();
				ThreadPoolJob currentJob = threadPool->GetJobQueue()[threadPool->GetJobCount()];
				threadPool->GetMutex().Unlock();

				currentJob.job(currentJob.args);
			}
			else
			{
				threadPool->GetMutex().Unlock();
			}
		}
	}

	ThreadPool::ThreadPool(uint32 maxThreads)
		: m_MaxThreads(maxThreads)
	{
		for (uint32 i = 0; i < maxThreads; i++)
		{
			m_WorkerThreads.PushBack(*new Thread(reinterpret_cast<Thread::StartRoutinePFN>(ThreadPoolWork), this));
		}
	}

	ThreadPool::~ThreadPool()
	{
		for (Thread& worker : m_WorkerThreads) 
		{
			worker.Join();
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