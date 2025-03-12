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
}