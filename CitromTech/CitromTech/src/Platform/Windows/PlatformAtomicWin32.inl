#pragma once

#include "WindowsIncludes.h"

namespace Citrom::Platform
{
    constexpr inline void SetAtomicMemoryOrderBarrierWin32(AtomicMemoryOrder order) noexcept
    {
        switch (order) 
        {
            case AtomicMemoryOrder::Relaxed: return; break;
            //case AtomicMemoryOrder::Acquire: _ReadBarrier(); break; // acquire and release require Each of these functions contains the word "Acquire" or "Release" in their names; for example, InterlockedDecrementAcquire and InterlockedDecrementRelease.
            //case AtomicMemoryOrder::Release: _WriteBarrier(); break;
            case AtomicMemoryOrder::AcqRel:  _ReadWriteBarrier(); break;
            case AtomicMemoryOrder::SeqCst:  MemoryBarrier(); break;

            default: return; break;
        }
    }

    template<typename T>
    void Atomic<T>::Store(T desired, AtomicMemoryOrder order)
    {
        SetAtomicMemoryOrderBarrierWin32(order);

        if constexpr(sizeof(T) == 8) // 64-bit
        {
            InterlockedExchange64(reinterpret_cast<volatile LONG64*>(&m_Value), static_cast<LONG64>(desired));
        }
        else // 32-bit
        {
            InterlockedExchange(reinterpret_cast<volatile LONG*>(&m_Value), static_cast<LONG>(desired));
        }
    }
    template<typename T>
    T Atomic<T>::Load(AtomicMemoryOrder order)
    {
        SetAtomicMemoryOrderBarrierWin32(order);

        if constexpr (sizeof(T) == 8) // 64-bit
        {
            return InterlockedCompareExchange64(reinterpret_cast<volatile LONG64*>(&m_Value), 0, 0);
        }
        else // 32-bit
        {
            return InterlockedCompareExchange(reinterpret_cast<volatile LONG*>(&m_Value), 0, 0);
        }
    }

    template<typename T>
    T Atomic<T>::Exchange(T desired, AtomicMemoryOrder order)
    {
        SetAtomicMemoryOrderBarrierWin32(order);

        if constexpr (sizeof(T) == 8) // 64-bit
        {
            return InterlockedExchange64(reinterpret_cast<volatile LONG64*>(&m_Value), static_cast<LONG64>(desired));
        }
        else // 32-bit
        {
            return InterlockedExchange(reinterpret_cast<volatile LONG*>(&m_Value), static_cast<LONG>(desired));
        }
    }
    template<typename T>
    bool Atomic<T>::CompareExchangeStrong(T& expected, T desired, AtomicMemoryOrder order)
    {
        SetAtomicMemoryOrderBarrierWin32(order);

        T old;
        if constexpr (sizeof(T) == 8) // 64-bit
        {
            old = InterlockedCompareExchange64(reinterpret_cast<volatile LONG64*>(&m_Value), static_cast<LONG64>(desired), static_cast<LONG64>(expected));
        }
        else // 32-bit
        {
            old = InterlockedCompareExchange(reinterpret_cast<volatile LONG*>(&m_Value), static_cast<LONG>(desired), static_cast<LONG>(expected));
        }

        if (old == expected)
            return true;

        expected = old;
        return false;
    }

    template<typename T>
    T Atomic<T>::FetchAdd(T arg, AtomicMemoryOrder order)
    {
        SetAtomicMemoryOrderBarrierWin32(order);

        if constexpr (sizeof(T) == 8) // 64-bit
        {
            return InterlockedExchangeAdd64(reinterpret_cast<volatile LONG64*>(&m_Value), static_cast<LONG64>(arg));
        }
        else // 32-bit
        {
            return InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(&m_Value), static_cast<LONG>(arg));
        }
    }
    template<typename T>
    inline T Atomic<T>::FetchSubtract(T arg, AtomicMemoryOrder order)
    {
        return FetchAdd(-arg, order);
    }

    template<typename T>
    T Atomic<T>::PreIncrement()
    {
        if constexpr (sizeof(T) == 8) // 64-bit
        {
            return InterlockedIncrement64(reinterpret_cast<volatile LONG64*>(&m_Value));
        }
        else // 32-bit
        {
            return InterlockedIncrement(reinterpret_cast<volatile LONG*>(&m_Value));
        }
    }
    template<typename T>
    inline T Atomic<T>::PostIncrement()
    {
        return FetchAdd(1);
    }

    template<typename T>
    T Atomic<T>::PreDecrement()
    {
        if constexpr (sizeof(T) == 8) // 64-bit
        {
            return InterlockedDecrement64(reinterpret_cast<volatile LONG64*>(&m_Value));
        }
        else // 32-bit
        {
            return InterlockedDecrement(reinterpret_cast<volatile LONG*>(&m_Value));
        }
    }
    template<typename T>
    inline T Atomic<T>::PostDecrement()
    {
        return FetchSubtract(1);
    }
}