#pragma once

#include "HashMap.h"

#include <unordered_set>

// CTL - (C)itrom Tech Standard (T)emplate (L)ibrary
namespace CTL
{
    // Hash Set wrapper, also known as
    // Unordered Set (C++)
    // HashSet (C#)
	template<typename K, typename Hash = XXHash<K>, typename Comparison = std::equal_to<K>>
    using HashSet = std::unordered_set<K, Hash, Comparison>;

    using CStrHashSet = HashSet<const char*, CStringHash, CStringHashEqual>;
    using StdStrHashSet = HashSet<std::string, StdStringHash, StdStringHashEqual>;

    template<typename K, typename T, typename Hash = XXHash<K>, typename Comparison = std::equal_to<K>>
    HashSet<K, Hash, Comparison> HashMapToHashSet(const HashMap<K, T, Hash, Comparison>& map)
    {
        HashSet<K, Hash, Comparison> set;
        for (const auto& [key, value] : map)
            set.insert(key);
        return set;
    }

    template<typename T>
    StdStrHashSet HashMapToHashSet(const StdStrHashMap<T>& map)
    {
        StdStrHashSet set;
        for (const auto& [key, value] : map)
            set.insert(key);
        return set;
    }

    template<typename T>
    CStrHashSet HashMapToHashSet(const CStrHashMap<T>& map)
    {
        CStrHashSet set;
        for (const auto& [key, value] : map)
            set.insert(key);
        return set;
    }
}