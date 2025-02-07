#pragma once

#include "Platform/PlatformThread.h"
#include "CitromAssert.h"
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
		void Submit(void(*job)(void* args), void* args) { Submit(static_cast<void*>(job), args); }

		/*template<typename IdxType = uint32>
		void ForEach(IdxType* iteratorBegin, IdxType* iteratorEnd, void(*callback)(IdxType index)/*, bool shouldWait*)
		{
			IdxType i = 0;
			for (auto it = iteratorBegin; it != iteratorEnd; ++it)
			{
				i++;
				Submit(callback, reinterpret_cast<void*>(&i));
			}
		}*/
		template<typename T>
		void ForEach(T* itBegin, T* itEnd, void(*callback)(T* iterator))
		{
			for (auto it = itBegin; it != itEnd; ++it)
			{
				Submit(callback, it);
			}
		}

		FORCE_INLINE uint32 GetJobCount() { return m_QueueJobs.Count(); }

		FORCE_INLINE Platform::Mutex& GetMutex() { return m_Mutex; }
		FORCE_INLINE CTL::DArray<ThreadPoolJob>* GetJobQueue() { return &m_QueueJobs; }
	private:
		uint32 m_MaxThreads;

		CTL::DArray<Platform::Thread> m_WorkerThreads;
		Platform::Mutex m_Mutex;

		CTL::DArray<ThreadPoolJob> m_QueueJobs;

		// dyn thread worker array
		// mutex
		// maxThreads
		// job list
	// Singleton Factory that is used by Citrom Tech and Citrom Forge
	public:
		static void Initialize(uint32 maxThreads)
		{
			s_Instance = new ThreadPool(maxThreads);
		}
		static ThreadPool* Get()
		{
			CT_CORE_ASSERT(s_Instance, "Trying to acquire an uninitialized Thread Pool!");
			return s_Instance;
		}
	private:
		static ThreadPool* s_Instance;
	};
}