#pragma once

#include "CTL.h"

#include "Vendor/xxHash/xxhash.h"
#include <unordered_map>

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
    // Hash Map wrapper, also known as
    // Unordered Map (C++)
    // Dictionary (C#)
    template<typename T>
    class XXHash
    {
    public:
        size_t operator()(const T& key) const 
        {
            return XXH64(&key, sizeof(T), 0); // XXH64 or XXH3_64bits
        }
    };

    // Custom Unordered_Map wrapper with xxHash as the default hash function
    template<typename K, typename T>
    using HashMap = std::unordered_map<K, T, XXHash<K>>;
}