#pragma once

#include "Core.h"

#include "CTL/String.h"

namespace Citrom
{
	namespace ArgumentHandler
	{
		// argv[0]
		void PushFilePath(const char* filePath);
		const char* GetFilePath();

		void PushArgument(const char* argument);

		bool HasArgument(const std::string& arg);

		std::string GetArgumentValue(const std::string& arg);
	}
}