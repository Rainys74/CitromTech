#pragma once

#include "CTL.h"

#include "Vendor/xxHash/xxhash.h"
#include <unordered_map>
#include <cstring>

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
            CTL_PROFILE_MEMBER_FUNC();

            //return XXH3_64bits(&key, sizeof(T));
            return XXH64(&key, sizeof(T), 0); // XXH64 or XXH3_64bits
        }
    };

    // Special CString Hash
    class CStringHash
    {
    public:
        size_t operator()(const char* str) const 
        {
            // Simple hash function (you can replace this with a better one, e.g., FNV-1a, etc.)
            size_t hash = 0;
            for (const char* c = str; *c != '\0'; ++c) 
            {
                hash = (hash * 31) + *c; // Simple hash algorithm
            }
            return hash;

            /*uint32 h;
            h ^= 2166136261UL;
            const uint8_t* data = (const uint8_t*)str;
            for (int i = 0; data[i] != '\0'; i++)
            {
                h ^= data[i];
                h *= 16777619;
            }
            return h;*/
        }
    };

    class CStringHashEqual
    {
    public:
        bool operator()(const char* strA, const char* strB) const
        {
            return std::strcmp(strA, strB) == 0;
        }
    };

    class StdStringHash
    {
    public:
        size_t operator()(const std::string& str) const
        {
            return CStringHash()(str.c_str());
        }
    };

    class StdStringHashEqual
    {
    public:
        bool operator()(const std::string& strA, const std::string& strB) const
        {
            return strA == strB;
        }
    };

    // Custom Unordered_Map wrapper with xxHash as the default hash function
    template<typename K, typename T, typename Hash = XXHash<K>, typename Comparison = std::equal_to<K>>
    using HashMap = std::unordered_map<K, T, Hash, Comparison>;

    template<typename T>
    using CStrHashMap = HashMap<const char*, T, CStringHash, CStringHashEqual>;

    template<typename T>
    using StdStrHashMap = HashMap<std::string, T, StdStringHash, StdStringHashEqual>;
}