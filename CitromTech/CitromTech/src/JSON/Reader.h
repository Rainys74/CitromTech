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

#define JSON_READER_COPY_STRING_VIEW_TO_STRING(STR, STRVIEW) (STR).assign((STRVIEW).data(), (STRVIEW).size()) // = std::string(_intern_StrView_ ## __LINE__) // maybe assign is better than copying?

#define JSON_READER_BEGIN()
#define JSON_READER_GET_STRING(KEY, STRING) std::string_view _intern_StrView_ ## __LINE__; JSON_READER_VERIFY(doc[(KEY)].get(_intern_StrView_ ## __LINE__)); JSON_READER_COPY_STRING_VIEW_TO_STRING(STRING, _intern_StrView_ ## __LINE__);
#define JSON_READER_DOC_GET_STRING(DOC, KEY, STRING) std::string_view _intern_StrView_ ## __LINE__; JSON_READER_VERIFY((DOC)[(KEY)].get(_intern_StrView_ ## __LINE__)); JSON_READER_COPY_STRING_VIEW_TO_STRING(STRING, _intern_StrView_ ## __LINE__);
#define JSON_READER_GET_VALUE(KEY, VALUE) doc[(KEY)].get(VALUE);
#define JSON_READER_GET_INT64(KEY, INT) JSON_READER_GET_VALUE(KEY, INT)
#define JSON_READER_GET_BOOL(KEY, BOOL) JSON_READER_GET_VALUE(KEY, BOOL)
#define JSON_READER_END()

#define JSON_READER_ODM_BEGIN() for (auto field : doc) { simdjson::ondemand::raw_json_string key; JSON_READER_VERIFY(field.key().get(key)); if (false) {}
#define JSON_READER_ODM_GET_STRING(KEY, STRING) else if (key == (KEY)) { field.value().get_string(STRING); }
#define JSON_READER_ODM_GET_INT64(KEY, INT) else if (key == (KEY)) {field.value().get(INT);}
#define JSON_READER_ODM_GET_BOOL(KEY, BOOL) else if (key == (KEY)) {field.value().get(BOOL);}
#define JSON_READER_ODM_END() }

	template<typename T>
	T DeserializeObjectOnDemand(const std::string& jsonString)
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

		return T::DeserializeJsonOnDemand(obj);
	}

	//static thread_local simdjson::dom::parser parser; // TODO: is something like this valid? doesn't global/static get cold and become slower to access?.. also static in header. (pretty sure works correctly inside a function)

	template<typename T>
	T DeserializeObject(const std::string& jsonString)
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
		//return DeserializeClass<T>(jsonPath.string());
		CT_PROFILE_GLOBAL_FUNCTION();

		simdjson::dom::parser parser; // reallocates memory
		simdjson::dom::element doc;

		//JSON_READER_VERIFY(parser.load_into_document(doc, jsonPath.string())); // TODO: does this work?

		return T::DeserializeJson(doc);
	}
}