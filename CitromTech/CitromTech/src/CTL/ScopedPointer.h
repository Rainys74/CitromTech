#pragma once

#include "CTL.h"

#include <memory>
#include <utility>

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
    // Scoped Pointer wrapper, also known as
    // Unique Pointer (C++)
    template <class T>
    struct ScopedPtrDelete 
    {
        void operator()(T* ptr) { delete ptr; }
    };

    template <class T, class D = ScopedPtrDelete<T>>
    class ScopedPtr 
    {
    public:
        ScopedPtr() noexcept : ScopedPtr{ nullptr } {}
        explicit ScopedPtr(T* ptr) noexcept : m_Data{ ptr }, m_Deleter{} {}
        ScopedPtr(T* ptr, const D& deleter) noexcept
            : m_Data{ ptr }, m_Deleter{ deleter } {}
        ScopedPtr(T* ptr, D&& deleter) noexcept
            : m_Data{ ptr }, m_Deleter{ std::move(deleter) } {}

        ScopedPtr(const ScopedPtr&) = delete;
        ScopedPtr& operator=(const ScopedPtr&) = delete;

        ScopedPtr(ScopedPtr&& other) noexcept
            : m_Data{ other.release() }, m_Deleter{ std::move(other.m_Deleter) } {}

        ScopedPtr& operator=(ScopedPtr&& other) noexcept 
        {
            if (this != &other) 
            {
                reset(other.release());
                m_Deleter = std::move(other.m_Deleter);
            }
            return *this;
        }

        explicit operator bool() const noexcept { return static_cast<bool>(m_Data); }

        T* Get() const noexcept { return m_Data; }
        T* operator->() const noexcept { return m_Data; }
        T& operator*() const noexcept { return *m_Data; }

        D& GetDeleter() noexcept { return m_Deleter; }
        const D& GetDeleter() const noexcept { return m_Deleter; }

        T* Release() noexcept { return std::exchange(m_Data, nullptr); }

        void Reset(T* ptr = nullptr) noexcept 
        {
            T* old = std::exchange(m_Data, ptr);
            if (old) 
            {
                m_Deleter(old);
            }
        }

        ~ScopedPtr() noexcept 
        {
            if (m_Data) 
            {
                m_Deleter(m_Data);
            }
        }

    private:
        NO_UNIQUE_ADDRESS D m_Deleter;
        T* m_Data;
    };
}