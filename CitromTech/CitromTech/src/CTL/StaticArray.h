#pragma once

#include "CTL.h"

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
    // Static Array (also known as a Fixed Array) Wrapper
    template<typename T, size_t S>
    class Array
    {
    public:
        constexpr size_t Length() const { return S; }

        // Operator overloading
        FORCE_INLINE T& operator[](size_t index) { return m_Data[index]; }
        FORCE_INLINE const T& operator[](size_t index) const { return m_Data[index]; }

        FORCE_INLINE T* Data() { return m_Data; }
        FORCE_INLINE const T* Data() const { return m_Data; }

        // Iterators
        FORCE_INLINE T* begin() { return &m_Data[0]; }
        FORCE_INLINE const T* begin() const { return &m_Data[0]; }
        FORCE_INLINE T* end() { return &m_Data[S]; }
        FORCE_INLINE const T* end() const { return &m_Data[S]; }

        //void Zero() {memset(&m_Data, 0, S * sizeof(T));}
    private:
        T m_Data[S];
    };
}