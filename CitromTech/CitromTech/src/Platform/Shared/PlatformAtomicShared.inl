#pragma once

namespace Citrom::Platform
{
    constexpr FORCE_INLINE std::memory_order /*_*/ AtomicMemoryOrderToSTD(AtomicMemoryOrder order) noexcept
    {
        switch (order)
        {
            case AtomicMemoryOrder::Relaxed: return std::memory_order_relaxed; break;
            case AtomicMemoryOrder::Consume: return std::memory_order_consume; break;
            case AtomicMemoryOrder::Acquire: return std::memory_order_acquire; break;
            case AtomicMemoryOrder::Release: return std::memory_order_release; break;
            case AtomicMemoryOrder::AcqRel: return std::memory_order_acq_rel; break;
            case AtomicMemoryOrder::SeqCst: return std::memory_order_seq_cst; break;

            default: return std::memory_order_seq_cst; break;
        }
        return std::memory_order_seq_cst; // Default case
    }

    template<typename T>
    void Atomic<T>::Store(T desired, AtomicMemoryOrder order)
    {
        m_Value.store(desired, AtomicMemoryOrderToSTD(order));
    }
    template<typename T>
    T Atomic<T>::Load(AtomicMemoryOrder order)
    {
        return m_Value.load(AtomicMemoryOrderToSTD(order));
    }

    template<typename T>
    T Atomic<T>::Exchange(T desired, AtomicMemoryOrder order)
    {
        return m_Value.exchange(desired, AtomicMemoryOrderToSTD(order));
    }
    template<typename T>
    bool Atomic<T>::CompareExchangeStrong(T& expected, T desired, AtomicMemoryOrder order)
    {
        return m_Value.compare_exchange_strong(expected, desired, AtomicMemoryOrderToSTD(order));
    }

    template<typename T>
    T Atomic<T>::FetchAdd(T arg, AtomicMemoryOrder order)
    {
        return m_Value.fetch_add(arg, AtomicMemoryOrderToSTD(order));
    }
    template<typename T>
    T Atomic<T>::FetchSubtract(T arg, AtomicMemoryOrder order)
    {
        return m_Value.fetch_sub(arg, AtomicMemoryOrderToSTD(order));
    }

    template<typename T>
    inline T Atomic<T>::PreIncrement()
    {
        return ++m_Value;
    }
    template<typename T>
    inline T Atomic<T>::PostIncrement()
    {
        return m_Value++;
    }

    template<typename T>
    inline T Atomic<T>::PreDecrement()
    {
        return --m_Value;
    }
    template<typename T>
    inline T Atomic<T>::PostDecrement()
    {
        return m_Value--;
    }
}