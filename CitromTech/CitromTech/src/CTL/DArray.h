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
        {
            m_Data = new T[count];
            m_Count = 0;
            m_Capacity = count;
        }
        ~DArray()
        {
            Clear();
        }

        void Resize(const uint32 newCount)
        {
            m_Capacity = newCount;
            m_Data = CTL_MEMORY_REALLOCATE(m_Data, m_Capacity * sizeof(T));
        }
        void Reserve(const uint32 count)
        {
            Resize(m_Capacity + count);
        }
        void PushBack(const T& value)
        {
            if (m_Count >= m_Capacity)
            {
                Reserve(m_Capacity * 2.0f);
            }
            m_Data[m_Count++] = value;
        }
        void PopBack()
        {
            m_Count -= 1;
        }

        void Clear()
        {
            delete[] m_Data;
            m_Count = 0;
            m_Capacity = 0;
        }

    private:
        T* m_Data;
        uint32 m_Count;
        uint32 m_Capacity;
    };
}