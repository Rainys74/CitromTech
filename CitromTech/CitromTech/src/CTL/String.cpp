#include "String.h"
#include "CStringHandling.h"

#include "CTL.h"

namespace CTL
{
    String::String()
    {
        m_Length = 0;
        m_Data = new char[m_Length];

        //str->length = 0;
        //str->data = memory_allocate(str->length, MEMORY_USAGE_TYPE_CONTAINER_STRINGV);
    }

    String::String(const char* initialString)
    {
        m_Length = CString::GetLength(initialString);
        m_Data = new char[CString::GetLength(initialString)];
        CTL_MEMORY_COPY(m_Data, initialString, m_Length);

        //str->length = cstring_get_length(initialString);
        //str->data = memory_allocate(cstring_get_length(initialString), MEMORY_USAGE_TYPE_CONTAINER_STRINGV);
        //memory_copy(str->data, initialString, str->length);
    }

    String::~String()
    {
        delete[] m_Data;
        m_Length = 0;

        //memory_free(str->data, str->length * sizeof(char), MEMORY_USAGE_TYPE_CONTAINER_STRINGV);
        //str->length = 0;
    }

    void String::Append(const char* data)
    {
        size_t appendLength = CString::GetLength(data);

        m_Data = (char*)CTL_MEMORY_ALLOCATE_ADD_MEMORY(m_Data, m_Length, appendLength);

        CTL_MEMORY_COPY(m_Data + m_Length, data, appendLength);

        m_Length += appendLength;

        //size_t appendLength = cstring_get_length(data);
        //
        //str->data = memory_allocate_add(str->data, str->length, appendLength, MEMORY_USAGE_TYPE_CONTAINER_STRINGV);
        //
        //memory_copy(str->data + str->length, data, appendLength);
        //
        //str->length += appendLength;
    }

    void String::PushBack(const char character)
    {
        if (m_Data == nullptr)
        {
            m_Data = new char[1];
            m_Data[0] = character;
            m_Length++;
        }
        else
        {
            m_Data = (char*)CTL_MEMORY_ALLOCATE_ADD_MEMORY(m_Data, m_Length, 1);
            m_Data[m_Length] = character;
            m_Length++;
        }

        //if (str->data == NULL)
        //{
        //    str->data = memory_allocate(sizeof(char) * 1, MEMORY_USAGE_TYPE_CONTAINER_STRINGV);
        //    str->data[0] = character;
        //    str->length++;
        //}
        //else
        //{
        //    str->data = memory_allocate_add(str->data, str->length * sizeof(char), 1 * sizeof(char), MEMORY_USAGE_TYPE_CONTAINER_STRINGV);
        //    str->data[str->length] = character;
        //    str->length++;
        //}
    }

    void String::Join(const String& secondString)
    {
        // TODO: implement
    }

    char* String::CStr() const
    {
        // TODO: this memory never gets freed
        char* cStr = new char[m_Length + 1];
        if (cStr)
        {
            CTL_MEMORY_COPY(cStr, m_Data, m_Length);
            cStr[m_Length] = '\0';

            return cStr;
        }

        return nullptr;

        //char* cStr = memory_allocate((strv->length + 1) * sizeof(char), MEMORY_USAGE_TYPE_CONTAINER_STRINGV);
        //if (cStr)
        //{
        //    memory_copy(cStr, strv->data, strv->length);
        //    cStr[strv->length] = '\0';
        //
        //    return cStr;
        //}
        //
        //CT_CORE_WARN("stringv wasn't converted to a C String properly.");
        //return NULL;
    }
    wchar_t* String::WStr()
    {
        // TODO: implement
        return nullptr;
    }
}