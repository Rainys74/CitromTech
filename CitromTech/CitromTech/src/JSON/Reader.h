#pragma once

#include "CTL/String.h"
#include "Profiling/Profiler.h"
#include "CitromAssert.h"

#include "Vendor/simdjson/simdjson.h"

#include <filesystem>

namespace Citrom::JSON // JSON vs Json
{
	// TODO: make a way to serialize and deserialize classes similarly to C# JsonSerializer.Deserialize<Settings>(Core.ReadFile(settingsPath)); JsonSerializer.Serialize<Settings>(settings, options); // options being a serializer descriptor
	
#define JSON_READER_VERIFY(x) if (auto error = (x) != simdjson::SUCCESS) { CT_CORE_ERROR("simdjson error: {}", simdjson::error_message(error)); CT_CORE_ASSERT(false, "simdjson failed!"); }

//#define JSON_READER_COPY_STRING_VIEW_TO_STRING(STR, STRVIEW) 

//#define JSON_READER_BEGIN()
//#define JSON_READER_GET_STRING(DOCKEY, STRING) std::string_view _intern_StrView_ ## __LINE__; JSON_READER_VERIFY(doc[(DOCKEY)].get(_intern_StrView_ ## __LINE__)); (STRING).assign(_intern_StrView_ ## __LINE__.data(), _intern_StrView_ ## __LINE__.size()) // = std::string(_intern_StrView_ ## __LINE__) // maybe assign is better than copying?
//#define JSON_READER_END()

#define JSON_READER_BEGIN() for (auto field : doc) { simdjson::ondemand::raw_json_string key; JSON_READER_VERIFY(field.key().get(key)); if (false) {}
#define JSON_READER_GET_STRING(DOCKEY, STRING) else if (key == (DOCKEY)) { /*std::string_view strView;*/ field.value().get_string(STRING); /*(STRING) = std::string(strView);*/ }
#define JSON_READER_GET_INT64(KEY, INT) else if (key == (KEY)) {JSON_READER_VERIFY(field.value().get(INT));}
#define JSON_READER_GET_BOOL(KEY, BOOL) else if (key == (KEY)) {JSON_READER_VERIFY(field.value().get(BOOL));}
#define JSON_READER_END() }

	struct TestClass
	{
		std::string name;
		int64 age;
		bool enabled;

		static TestClass DeserializeJson(simdjson::ondemand::object doc)
		{
			TestClass testClass;

			//doc["name"].get(testClass.name);
			//doc["age"].get(testClass.age);
			//doc["enabled"].get(testClass.enabled);
			//std::string_view nameView;
			//doc["name"].get(nameView);
			//testClass.name = std::string(nameView);
			
			JSON_READER_BEGIN()

			JSON_READER_GET_STRING("name", testClass.name)
			JSON_READER_GET_INT64("age", testClass.age)
			JSON_READER_GET_BOOL("enabled", testClass.enabled)

			JSON_READER_END();

			return testClass;
		}
	};

	template<typename T>
	T DeserializeClass(const std::string& jsonString) // object?
	{
		CT_PROFILE_GLOBAL_FUNCTION();

		simdjson::padded_string paddedString(jsonString);

		simdjson::ondemand::parser parser;
		simdjson::ondemand::document doc = parser.iterate(paddedString);
		/*try {
			doc = parser.iterate(jsonString);
		} catch (const simdjson::simdjson_error& e) {
			CT_CORE_ERROR("Simdjson parsing error: {}", e.what());
			CT_CORE_ASSERT(false, "Simdjson parsing failed!");
		}*/

		//JSON_READER_VERIFY(parser.iterate(jsonString).get(doc));

		simdjson::ondemand::object obj;
		JSON_READER_VERIFY(doc.get_object().get(obj));
		return T::DeserializeJson(obj);
	}

	template<typename T>
	T DeserializeClass(const std::filesystem::path& jsonPath)
	{
		//return DeserializeClass<T>(jsonPath.string());
		CT_PROFILE_GLOBAL_FUNCTION();

		simdjson::dom::parser parser; // reallocates memory
		simdjson::dom::element doc;

		//JSON_READER_VERIFY(parser.load_into_document(doc, jsonPath.string())); // TODO: does this work?

		return T::DeserializeJson(doc);
	}
}