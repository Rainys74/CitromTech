#pragma once

#include "CTL/String.h"
#include "Profiling/Profiler.h"

#include "Vendor/rapidjson/document.h"
#include "Vendor/rapidjson/writer.h"
#include "Vendor/rapidjson/prettywriter.h"

#include <stack>

namespace Citrom::JSON // JSON vs Json
{
    //doc.AddMember("name", rapidjson::Value(this->name.c_str(), allocator), allocator);
            //doc.AddMember("age", this->age, allocator);
            //doc.AddMember("enabled", this->enabled, allocator);

#define JSON_WRITER_SET_STRING(KEY, STRING) doc.AddMember((KEY), rapidjson::Value((STRING).c_str(), allocator), allocator)

#define JSON_WRITER_SET_VALUE(KEY, VALUE) doc.AddMember((KEY), (VALUE), allocator)

#define JSON_WRITER_SET_INT64(KEY, INT) JSON_WRITER_SET_VALUE(KEY, INT)
#define JSON_WRITER_SET_BOOL(KEY, BOOL) JSON_WRITER_SET_VALUE(KEY, BOOL)

    struct SerializerOptions
    {
        bool prettify = false;

        SerializerOptions(bool _prettify = false)
            : prettify(_prettify) {}
    };

    template<typename T>
    std::string SerializeObject(T& object, const SerializerOptions& options = SerializerOptions()) // nullptr or modern C++? modern C++ pluses are you can generate an options object on the fly
    {
        CT_PROFILE_GLOBAL_FUNCTION();

        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        object.SerializeJson(doc, allocator);

        // Convert to string
        rapidjson::StringBuffer buffer;

        if (!options.prettify)
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            doc.Accept(writer);
        }
        else
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            //writer.SetIndent(options.indentChar, options.indentSize);
            doc.Accept(writer);
        }

        return buffer.GetString();
    }

    /*class BuilderWriter
    {
    public:
        BuilderWriter()
            : allocator(doc.GetAllocator())
        {
            doc.SetObject();
            allocator = doc.GetAllocator();
        }

        BuilderWriter& Key(const std::string& key)
        {
            currentKey = key;
            return *this;
        }

        BuilderWriter& Value(const std::string& value)
        {
            doc.AddMember(rapidjson::Value(currentKey.c_str(), allocator), rapidjson::Value(value.c_str(), allocator), allocator);
            return *this;
        }

        BuilderWriter& Value(int64 value)
        {
            doc.AddMember(rapidjson::Value(currentKey.c_str(), allocator), rapidjson::Value(value), allocator);
            return *this;
        }

        BuilderWriter& Value(double value)
        {
            doc.AddMember(rapidjson::Value(currentKey.c_str(), allocator), rapidjson::Value(value), allocator);
            return *this;
        }

        BuilderWriter& Value(bool value)
        {
            doc.AddMember(rapidjson::Value(currentKey.c_str(), allocator), rapidjson::Value(value), allocator);
            return *this;
        }

        BuilderWriter& BeginMap()
        {
            objectStack.push(rapidjson::Value(rapidjson::kObjectType));
            return *this;
        }

        BuilderWriter& EndMap()
        {
            /*if (!objectStack.empty())
            {
                rapidjson::Value obj = std::move(objectStack.top());
                objectStack.pop();
                doc.AddMember(rapidjson::Value(currentKey.c_str(), allocator), obj, allocator);
            }*/
            /*if (objectStack.size() > 1) // Prevent removing root
            { 
                rapidjson::Value* obj = &objectStack.top();
                objectStack.pop();
                AddMember(std::move(*obj));
                //delete obj;
            }
            return *this;* /
            if (!objectStack.empty())
            {
                // Pop the current object and add it to the parent level
                rapidjson::Value obj = std::move(objectStack.top());
                objectStack.pop();

                // If we're at the root level, add the object to the document directly
                if (objectStack.empty())
                {
                    doc.AddMember(rapidjson::Value(currentKey.c_str(), allocator), obj, allocator);
                }
                else
                {
                    // Otherwise, add the object to the parent object in the stack
                    objectStack.top().AddMember(rapidjson::Value(currentKey.c_str(), allocator), obj, allocator);
                }
                currentKey.clear();
            }
            return *this;
        }

        std::string GetString()
        {
            rapidjson::StringBuffer buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            doc.Accept(writer);
            return buffer.GetString();
        }
    private:
        void AddMember(rapidjson::Value value) 
        {
            if (!currentKey.empty() && !objectStack.empty()) 
            {
                rapidjson::Value key(currentKey.c_str(), allocator);
                objectStack.top().AddMember(key, value, allocator);
                currentKey.clear();
            }
        }
    private:
        rapidjson::Document doc;
        rapidjson::Document::AllocatorType& allocator;
        std::stack<rapidjson::Value> objectStack;
        std::string currentKey;
    };*/
}