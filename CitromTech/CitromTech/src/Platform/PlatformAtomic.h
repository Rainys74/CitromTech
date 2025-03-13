#pragma once

#include "Core.h"

#if !defined(UNDEFINED_MACRO_WINDOWS_TEST)
#include <atomic>
#endif

namespace Citrom::Platform
{
	enum class AtomicMemoryOrder
	{
		SeqCst, // Sequential Consistency

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
		constexpr Atomic() {}

		// Delete copy constructor and assignment (like std::atomic)
		Atomic(const Atomic&) = delete;
		Atomic& operator=(const Atomic&) = delete;

		void Store(T desired, AtomicMemoryOrder order = AtomicMemoryOrder::SeqCst);
		T Load(AtomicMemoryOrder order = AtomicMemoryOrder::SeqCst);

		T Exchange(T desired, AtomicMemoryOrder order = AtomicMemoryOrder::SeqCst);
		bool CompareExchangeStrong(T& expected, T desired, AtomicMemoryOrder order = AtomicMemoryOrder::SeqCst); // Compare and exchange

		T FetchAdd(T arg, AtomicMemoryOrder order = AtomicMemoryOrder::SeqCst);
		T FetchSubtract(T arg, AtomicMemoryOrder order = AtomicMemoryOrder::SeqCst);

		// Pre or PrefixIncrement
		T PreIncrement(); // ++atomic
		T PostIncrement(); // atomic++

		T PreDecrement(); // --atomic
		T PostDecrement(); // atomic--

		/*
		FORCE_INLINE T operator++() { return PreIncrement(); } // Prefix increment // TODO: pretty sure pre-increments should return a reference, so maybe instead you have to do "Increment(); return *this;"?
		FORCE_INLINE T operator++(int) { return PostIncrement(); } // Postfix increment

		FORCE_INLINE T operator--() { return PreDecrement(); } // Prefix decrement
		FORCE_INLINE T operator--(int) { return PostDecrement(); } // Postfix decrement

		//FORCE_INLINE operator T() const { return Load(); } // Implicit conversion to T
		*/
	private:
#ifdef CT_PLATFORM_WINDOWS
		volatile T m_Value;
#else
		std::atomic<T> m_Value;
#endif
	};
}

#ifdef CT_PLATFORM_WINDOWS
#include "Windows/PlatformAtomicWin32.inl"
#else
#include "Shared/PlatformAtomicShared.inl"
#endif