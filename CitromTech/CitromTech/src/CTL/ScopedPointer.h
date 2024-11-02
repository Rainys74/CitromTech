#pragma once

#include "CTL.h"

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
    // TODO: temporary
    #include <memory>
    template<typename T>
    using ScopedPtr = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr ScopedPtr<T> CreateScopedPtr(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // Scoped Pointer wrapper, also known as
    // Unique Pointer (C++)
    /*template<typename T>
    class ScopedPtr
    {

    };*/
}