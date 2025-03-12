#pragma once

#include "Core.h"

#if !defined(UNDEFINED_MACRO_WINDOWS_TEST)
#include <atomic>
#endif

namespace Citrom::Platform
{
	enum class AtomicMemoryOrder
	{
		SeqCst,

		Relaxed,
		Consume,
		Acquire,
		Release,
		AcqRel
	};

	template<typename T>
	class Atomic
	{
	public:
		// Delete copy constructor and assignment (like std::atomic)
		Atomic(const Atomic&) = delete;
		Atomic& operator=(const Atomic&) = delete;

		//void Store(T desired, int order = 0);
		//T Load(int order = 0);

	private:
#ifdef UNDEFINED_MACRO_WINDOWS_TEST
		volatile T m_Value;
#else
		std::atomic<T> m_Value;
#endif
	};
}

#ifdef UNDEFINED_MACRO_WINDOWS_TEST
#else
#include "Shared/PlatformAtomicShared.inl"
#endif