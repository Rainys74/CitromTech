#pragma once

#include "Core.h"

namespace Citrom::Platform
{
	class Thread
	{
	public:
		using StartRoutinePFN = uint32(*)(void*);

		Thread(StartRoutinePFN startRoutine, void* args);
		~Thread();

		void Join(); // waits for the thread to catch up
		void Wait(uint64 ms); // waits for a specific time amount

		void Detach();
		void TryCancel();

		bool IsActive();

		void Sleep(uint64 ms);

		// Returns the ID of the thread being executed on
		static uint64 GetCurrentID();
	private:
		uint64 m_ID = 0;
		void* m_Internal = nullptr;
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
	};
}