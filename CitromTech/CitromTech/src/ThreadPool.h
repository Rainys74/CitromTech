#pragma once

#include "Platform/PlatformThread.h"
#include "Platform/PlatformAtomic.h"
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
		void Submit(void(*job)(void* args), void* args) { Submit(reinterpret_cast<void*>((uintptr)job), args); }

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
				Submit((void(*)(void*))callback, it);
			}
		}

		inline bool IsBusy()
		{
			return static_cast<bool>(m_FinishedLabel.Load() < m_CurrentLabel);
		}

		FORCE_INLINE uint32 GetJobCount() { return m_QueueJobs.Count(); }

		FORCE_INLINE Platform::Mutex& GetMutex() { return m_Mutex; }
		FORCE_INLINE CTL::DArray<ThreadPoolJob>* GetJobQueue() { return &m_QueueJobs; }
		FORCE_INLINE Platform::Atomic<uint64>& GetAtomicFinishedLabel() { return m_FinishedLabel; }
		FORCE_INLINE Platform::ConditionVariable& GetCondition() { return m_Condition; }
	private:
		uint32 m_MaxThreads;

		CTL::DArray<Platform::Thread> m_WorkerThreads;
		Platform::Mutex m_Mutex;
		Platform::ConditionVariable m_Condition;

		CTL::DArray<ThreadPoolJob> m_QueueJobs;

		uint64 m_CurrentLabel = 0;
		Platform::Atomic<uint64> m_FinishedLabel;

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
