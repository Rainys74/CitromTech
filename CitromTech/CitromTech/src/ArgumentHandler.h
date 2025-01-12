#pragma once

#include "Core.h"

#include "CTL/String.h"
#include "CTL/DArray.h"

namespace Citrom
{
	namespace ArgumentHandler
	{
		// argv[0]
		void PushFilePath(const char* filePath);
		const char* GetFilePath();

		// Allocates an array of arguments once and doesn't free them
		// should be used once per the entirety of the program to not
		// cause memory leaks
		//char** GetArgumentsFromFile(const std::string& filePath);
		CTL::DArray<std::string> GetArgumentsFromFile(const std::string& filePath);

		void PushArgument(const char* argument);

		bool HasArgument(const std::string& arg);

		std::string GetArgumentValue(const std::string& arg);
	}
}