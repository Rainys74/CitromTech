#pragma once

#include "Core.h"

namespace Citrom::Platform
{
	class Thread
	{
	public:
		using StartRoutinePFN = uint32(*)(void*);

		Thread();
		Thread(StartRoutinePFN startRoutine, void* args);
		~Thread();

		void Initialize(const StartRoutinePFN startRoutine, void* args); // Start

		void Join(); // waits for the thread to catch up (Wait)
		void Wait(uint64 ms); // waits for a specific time amount (WaitTimeout)

		void Detach();
		void TryCancel();

		bool IsActive();

		void Sleep(uint64 ms);

		// Returns the ID of the thread being executed on
		static uint64 GetCurrentID();
	private:
		uint64 m_ID = 0;
		void* m_Internal = nullptr;

		bool m_Initialized = false; // wastes 32 bits (4 bytes) but oh well
	};

	class Mutex
	{
	public:
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();
	private:
		void* m_Internal = nullptr;
        
        friend class ConditionVariable;
	};

	class ScopedMutex
	{
	public:
		ScopedMutex()
		{
			m_Mutex.Lock();
		}
		~ScopedMutex()
		{
			m_Mutex.Unlock();
		}
	private:
		Mutex m_Mutex;
	};

	// Scoped Mutex Wrapper
	class ScopedLock
	{
	public:
		ScopedLock(Mutex& mutex)
			: m_Mutex(mutex)
		{
			m_Mutex.Lock();
		}
		~ScopedLock()
		{
			m_Mutex.Unlock();
		}
	private:
		Mutex& m_Mutex;
	};

	// TODO: test
	class ConditionVariable
	{
	public:
        ConditionVariable();
		~ConditionVariable();

		void Wait(Mutex& mutex);
		void NotifyOne();
		void NotifyAll();

	private:
		void* m_Internal = nullptr;
	};
}
