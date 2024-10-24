#include "CStringHandling.h"
#include "CTL.h"

#include <cstring>

namespace CTL::CString
{
    size_t GetLength(const char* string)
    {
        return std::strlen(string);
        /*if (string == NULL)
            return 0;
        uint32 length = 0;
        while (string[length] != '\0')
        {
            length++;
        }
        return length;*/
    }

    bool IsEmptyOrNull(const char* string)
    {
        return (string == NULL || string[0] == '\0');
    }

    bool IsEmpty(const char* string)
    {
        return (string[0] == '\0');
    }

    char* Copy(char* destination, const char* source)
    {
        CTL_MEMORY_COPY(destination, source, GetLength(source) + 1);
        return destination;
    }

    char* Concatenate(char* destination, const char* source)
    {
        return std::strcat(destination, source);
    }

    char* ReverseSearchForCharacter(const char* string, const char character)
    {
        // TODO: fix this
        return nullptr; //std::strrchr(string, character);
    }
}