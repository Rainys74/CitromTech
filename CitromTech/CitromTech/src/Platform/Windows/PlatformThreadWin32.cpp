#include "Platform/PlatformThread.h"
#include "Platform/Platform.h" // Utils::Sleep

#ifdef CT_PLATFORM_WINDOWS
#include "WindowsIncludes.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"

namespace Citrom::Platform
{
	Thread::Thread()
		: m_ID(0), m_Internal(nullptr)
	{
	}
	Thread::Thread(StartRoutinePFN startRoutine, void* args)
		: m_ID(0), m_Internal(nullptr)
	{
		CT_CORE_ASSERT(startRoutine, "No function pointer passed into a thread!");

		m_Internal = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)startRoutine, args, 0, (DWORD*)&m_ID);

		CT_CORE_TRACE("Starting process on thread ID: {}", m_ID);
		CT_CORE_ASSERT(m_Internal, "Failed to create thread!");
	}
	Thread::~Thread()
	{
		if (m_Internal == nullptr)
			return;
		// TODO: fix this memory leak someday
		//DWORD exitCode;
		//GetExitCodeThread(m_Internal, &exitCode);
		//
		//// TerminateThread() is a great way to end up with dead locked mutexes, broken data, etc.
		///*if (exitCode == STILL_ACTIVE)
		//	TerminateThread(m_Internal, 0);*/
		//
		//CloseHandle((HANDLE)m_Internal);
	}
	void Thread::Initialize(const StartRoutinePFN startRoutine, void* args)
	{
		CT_CORE_ASSERT(startRoutine, "No function pointer passed into a thread!");

		m_Internal = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)startRoutine, args, 0, (DWORD*)&m_ID);

		CT_CORE_TRACE("Starting process on thread ID: {}", m_ID);
		CT_CORE_ASSERT(m_Internal, "Failed to create thread!");
	}
	void Thread::Join()
	{
		DWORD exitCode = WaitForSingleObject(m_Internal, INFINITE);
		CT_CORE_ASSERT(exitCode == WAIT_OBJECT_0, "Failed to join thread: WaitForSingleObject returned an unexpected exit code.");
	}
	void Thread::Wait(uint64 ms)
	{
		DWORD exitCode = WaitForSingleObject(m_Internal, static_cast<DWORD>(ms));
		if (exitCode == WAIT_TIMEOUT)
		{
			return;
		}
		CT_CORE_ASSERT(exitCode == WAIT_OBJECT_0, "Failed to wait for thread: WaitForSingleObject returned an unexpected exit code.");
	}
	void Thread::Detach()
	{
		CloseHandle(m_Internal);
	}
	void Thread::TryCancel()
	{
		// TerminateThread() is a great way to end up with dead locked mutexes, broken data, etc.
		TerminateThread(m_Internal, 0);
	}
	bool Thread::IsActive()
	{
		DWORD exitCode = WaitForSingleObject(m_Internal, 0);
		if (exitCode == WAIT_TIMEOUT)
		{
			return true;
		}
		return false;
	}
	void Thread::Sleep(uint64 ms)
	{
		//Platform::Sleep(ms);
		Platform::Utils::Sleep(ms);
	}
	uint64 Thread::GetCurrentID()
	{
		return (uint64)GetCurrentThreadId();
	}

	Mutex::Mutex()
		: m_Internal(nullptr)
	{
		m_Internal = CreateMutex(0, 0, 0);
		CT_CORE_ASSERT(m_Internal, "Failed to create mutex!");
	}
	Mutex::~Mutex()
	{
		CloseHandle(m_Internal);
	}

	void Mutex::Lock()
	{
		DWORD result = WaitForSingleObject(m_Internal, INFINITE);
		switch (result)
		{
		case WAIT_OBJECT_0:
			return; // all good
			break;
		case WAIT_ABANDONED:
			CT_CORE_ASSERT(false, "The thread got ownership of an abandoned mutex.");
			break;
		}
	}
	void Mutex::Unlock()
	{
		int32 result = ReleaseMutex(m_Internal);
		CT_CORE_ASSERT(result != 0, "Failed to unlock mutex!"); // 0 == Error
	}
}
#endif