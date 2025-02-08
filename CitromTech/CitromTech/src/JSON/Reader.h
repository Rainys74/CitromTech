#pragma once

#include "CTL/String.h"
#include "Profiling/Profiler.h"

#include "Vendor/simdjson/simdjson.h"

#include <filesystem>

namespace Citrom::JSON // JSON vs Json
{
	// TODO: make a way to serialize and deserialize classes similarly to C# JsonSerializer.Deserialize<Settings>(Core.ReadFile(settingsPath)); JsonSerializer.Serialize<Settings>(settings, options); // options being a serializer descriptor
	
#define JSON_READER_VERIFY(x) CT_ASSERT((x) == simdjson::SUCCESS, "simdjson failed!")
#define JSON_READER_GET_STRING(DOC, STRING) std::string_view _intern_StrView_ ## __LINE__; JSON_READER_VERIFY((DOC).get(_intern_StrView_ ## __LINE__)); (STRING).assign(_intern_StrView_ ## __LINE__.data(), _intern_StrView_ ## __LINE__.size()) // = std::string(_intern_StrView_ ## __LINE__) // maybe assign is better than copying?

	struct TestClass
	{
		std::string name;
		int64 age;
		bool enabled;

		static TestClass DeserializeJson(const simdjson::dom::element& doc)
		{
			TestClass testClass;

			//doc["name"].get(testClass.name);
			//doc["age"].get(testClass.age);
			//doc["enabled"].get(testClass.enabled);
			//std::string_view nameView;
			//doc["name"].get(nameView);
			//testClass.name = std::string(nameView);
			JSON_READER_GET_STRING(doc["name"], testClass.name);
			JSON_READER_VERIFY(doc["age"].get(testClass.age));
			JSON_READER_VERIFY(doc["enabled"].get(testClass.enabled));

			return testClass;
		}
	};

	template<typename T>
	T DeserializeClass(const std::string& jsonString) // object?
	{
		CT_PROFILE_GLOBAL_FUNCTION();

		simdjson::dom::parser parser; // reallocates memory
		simdjson::dom::element doc;

		JSON_READER_VERIFY(parser.parse(jsonString).get(doc));

		return T::DeserializeJson(doc);
	}

	template<typename T>
	T DeserializeClass(const std::filesystem::path& jsonPath)
	{
		DeserializeClass<T>(jsonPath.string());
	}
}