#pragma once

#include "CTL.h"

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
    // Dynamic Array wrapper, also known as
    // Vector (C++)
    // List (C#)
    template<typename T>
    class DArray
    {
    public:
        DArray(const uint32 count = 1)
            : m_Data(nullptr), m_Count(0), m_Capacity(count)
        {
            m_Data = new T[count];
        }
        ~DArray()
        {
            Clear();
        }

        void Resize(const uint32 newCapacity)
        {
            CTL_PROFILE_MEMBER_FUNC();

            T* newData = new T[newCapacity];

            if (newCapacity < m_Count)
                m_Count = newCapacity;

            // Move/Copy data this way to call the constructors
            for (uint32 i = 0; i < m_Count; i++)
            {
                newData[i] = m_Data[i];
            }

            delete[] m_Data;
            m_Data = newData;
            m_Capacity = newCapacity;
        }
        void Reserve(const uint32 count)
        {
            Resize(m_Capacity + count);
        }
        void PushBack(const T& value)
        {
            if (m_Count >= m_Capacity)
            {
                // the 2.0f might be templated to allow for configuration
                Resize(m_Capacity * 2);
            }
            m_Data[m_Count++] = value;
        }
        void PopBack()
        {
            // or write as m_Data[--m_Count].~T();
            m_Data[m_Count-1].~T();
            m_Count -= 1;
        }

        void Clear()
        {
            delete[] m_Data;
            m_Count = 0;
            m_Capacity = 0;
        }

        FORCE_INLINE T* Data() { return m_Data; }
        FORCE_INLINE uint32 Count() { return m_Count; }
        FORCE_INLINE uint32 Capacity() { return m_Capacity; }

        // Operator Overloading
        FORCE_INLINE const T& operator[](size_t index) const {return m_Data[index];}
        FORCE_INLINE T& operator[](size_t index) {return m_Data[index];}

        // Iterators
        FORCE_INLINE T* begin() { return &m_Data[0]; }
        FORCE_INLINE const T* begin() const { return &m_Data[0]; }
        FORCE_INLINE T* end() { return &m_Data[m_Count]; }
        FORCE_INLINE const T* end() const { return &m_Data[m_Count]; }

    private:
        T* m_Data;
        uint32 m_Count;
        uint32 m_Capacity;
    };
}