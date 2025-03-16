#include "Platform/PlatformThread.h"
#include "Platform/Platform.h"

#ifdef CT_PLATFORM_POSIX
#include <pthread.h>

#include "Logger/Logger.h"
#include "CitromAssert.h"

#ifndef CT_DEBUG
#define PTHREAD_VERIFY(x) x
#else
#include <errno.h>
#include <string.h>
#define PTHREAD_VERIFY(x) int _Intern_result ## __LINE__ = x; 											\
						  if (_Intern_result ## __LINE__ != 0) 											\
						  {																				\
							CT_CORE_FATAL("pthread error: {}", strerror(_Intern_result ## __LINE__));	\
							CT_CORE_ASSERT(false, "Failed to "#x"!");									\
						  }
#endif

namespace Citrom::Platform
{
	Thread::Thread()
		: m_ID(0), m_Internal(nullptr) {}
	Thread::Thread(StartRoutinePFN startRoutine, void* args)
		: m_ID(0), m_Internal(nullptr)
	{
		Initialize(startRoutine, args);
	}
	Thread::~Thread()
	{
		if (m_Initialized)
        {
            pthread_detach((pthread_t)m_Internal); // Detach if not joined
        }
	}
	void Thread::Initialize(const StartRoutinePFN startRoutine, void* args)
	{
		CT_CORE_ASSERT(startRoutine, "No function pointer passed into a thread!");

        int result = pthread_create((pthread_t*)&m_Internal, nullptr, reinterpret_cast<void* (*)(void*)>(startRoutine), args);
        CT_CORE_ASSERT(result == 0, "Failed to create thread!");

        m_Initialized = true;
		m_ID = (uint64)((pthread_t)m_Internal);
	}

	void Thread::Join()
	{
		PTHREAD_VERIFY(pthread_join((pthread_t)m_Internal, nullptr));
	}
	void Thread::Wait(uint64 ms)
	{
	}
	void Thread::Detach()
	{
		PTHREAD_VERIFY(pthread_detach((pthread_t)m_Internal));
	}
	void Thread::TryCancel()
	{
		PTHREAD_VERIFY(pthread_cancel((pthread_t)m_Internal));
	}
	bool Thread::IsActive()
	{
        return pthread_kill((pthread_t)m_Internal, 0) == 0;
	}
	void Thread::Sleep(uint64 ms)
	{
		Platform::Utils::Sleep(ms);
	}
	uint64 Thread::GetCurrentID()
	{
		return (uint64)pthread_self();
	}

	Mutex::Mutex()
		: m_Internal(nullptr)
	{
		//PTHREAD_VERIFY(EINVAL); // pthread error: Invalid argument
		PTHREAD_VERIFY(pthread_mutex_init((pthread_mutex_t*)&m_Internal, nullptr));
	}
	Mutex::~Mutex()
	{
		PTHREAD_VERIFY(pthread_mutex_destroy((pthread_mutex_t*)&m_Internal));
	}

	void Mutex::Lock()
	{
		PTHREAD_VERIFY(pthread_mutex_lock((pthread_mutex_t*)&m_Internal));
	}
	void Mutex::Unlock()
	{
		PTHREAD_VERIFY(pthread_mutex_unlock((pthread_mutex_t*)&m_Internal));
	}

	// Condition Variable
	ConditionVariable::ConditionVariable()
	{
		PTHREAD_VERIFY(pthread_cond_init((pthread_cond_t*)&m_Internal, nullptr));
	}

	ConditionVariable::~ConditionVariable()
	{
		PTHREAD_VERIFY(pthread_cond_destroy((pthread_cond_t*)&m_Internal));
	}

	void ConditionVariable::Wait(Mutex& mutex)
	{
		PTHREAD_VERIFY(pthread_cond_wait((pthread_cond_t*)&m_Internal, (pthread_mutex_t*)&mutex.m_Internal));
	}

	void ConditionVariable::NotifyOne()
	{
		PTHREAD_VERIFY(pthread_cond_signal((pthread_cond_t*)&m_Internal));
	}

	void ConditionVariable::NotifyAll()
	{
		PTHREAD_VERIFY(pthread_cond_broadcast((pthread_cond_t*)&m_Internal));
	}
}
#endif
