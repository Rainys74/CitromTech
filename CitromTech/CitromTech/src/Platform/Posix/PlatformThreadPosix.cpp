#include "Platform/PlatformThread.h"

#ifdef CT_PLATFORM_POSIX
#include <pthread.h>

#include "Logger/Logger.h"
#include "CitromAssert.h"

namespace Citrom::Platform
{
	Thread::Thread(StartRoutinePFN startRoutine, void* args)
		: m_ID(0), m_Internal(nullptr)
	{
		
	}
	Thread::~Thread()
	{
		
	}
	void Thread::Join()
	{
		
	}
	void Thread::Wait(uint64 ms)
	{
		
	}
	void Thread::Detach()
	{
		
	}
	void Thread::TryCancel()
	{
		
	}
	bool Thread::IsActive()
	{
		
	}
	void Thread::Sleep(uint64 ms)
	{
		//Platform::Sleep(ms);
	}
	uint64 Thread::GetCurrentID()
	{
		
	}

	Mutex::Mutex()
		: m_Internal(nullptr)
	{
		
	}
	Mutex::~Mutex()
	{
		
	}

	void Mutex::Lock()
	{
		
	}
	void Mutex::Unlock()
	{
		
	}
}
#endif