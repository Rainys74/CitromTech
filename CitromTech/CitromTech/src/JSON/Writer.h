#pragma once

#include "CTL/String.h"
#include "Profiling/Profiler.h"

#include "Vendor/simdjson/simdjson.h"

#include <filesystem>

namespace Citrom::JSON // JSON vs Json
{
	// TODO: make a way to serialize and deserialize classes similarly to C# JsonSerializer.Deserialize<Settings>(Core.ReadFile(settingsPath)); JsonSerializer.Serialize<Settings>(settings, options); // options being a serializer descriptor
	struct TestClass
	{
		std::string name;
		int64 age;
		bool enabled;

		static std::string SerializeJson()
		{
		}
	};

	template<typename T>
	std::string SerializeObject(const T& object)
	{
		CT_PROFILE_GLOBAL_FUNCTION();

	}
}