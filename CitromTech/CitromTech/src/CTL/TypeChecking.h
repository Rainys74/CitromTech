#pragma once

#include "CTL.h"

namespace CTL
{
    // Custom Type Traits
    template<typename T, typename U>
    struct IsSame
    {
        static constexpr bool value = false;

        constexpr bool operator()() const
        {
            return value;
        }
    };

    // Specialization for when types are the same
    template<typename T>
    struct IsSame<T, T>
    {
        static constexpr bool value = true;

        constexpr bool operator()() const
        {
            return value;
        }
    };

    // conditional_t replacement
    template<bool Condition, typename TrueType, typename FalseType>
    struct Conditional
    {
        using type = TrueType;
    };

    template<typename TrueType, typename FalseType>
    struct Conditional<false, TrueType, FalseType>
    {
        using type = FalseType;
    };
}
