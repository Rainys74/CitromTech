#include "ArgumentHandler.h"
#include "CitromAssert.h"

#include "CTL/HashMap.h"
#include "CTL/String.h"

#include "Platform/Platform.h"

#include <fstream>

namespace Citrom::ArgumentHandler
{
	// Argument - Value
	static CTL::HashMap<std::string, std::string, CTL::StdStringHash> g_Arguments;

	CTL::DArray<std::string> GetArgumentsFromFile(const std::string& filePath)
	{
		CTL::DArray<std::string> args;

		CT_CORE_WARN("Current WORKING DIR: {}", Platform::Utils::GetWorkingDirectory());
		CT_CORE_VERIFY(Platform::Utils::RequestAccessToFile(filePath.c_str()), "Access was denied, cannot continue!");
		std::ifstream file(filePath);
		CT_CORE_ASSERT(file.is_open(), "Failed to open a file for reading command line arguments!");

		// Read the file into a string
		std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Split the file content by spaces while trimming extra whitespace
		std::istringstream stream(fileContent);
		std::string token;
		while (stream >> token)
		{
			args.PushBack(token);
		}

		return args;
	}

	void PushArgument(const char* argument)
	{
		// splits the string from delimiter "=" to two parts (key, value)
		CTL::DArray<std::string> splitString = CTL::StdString::Split(argument, '=');

		if (splitString.Count() == 2)
		{
			CT_CORE_VERBOSE("Argument ({}) found, pushing it as [{}, {}]", argument, splitString[0], splitString[1]);
			g_Arguments[splitString[0]] = splitString[1];
		}
		else if (splitString.Count() == 1)
		{
			CT_CORE_VERBOSE("Argument ({}) found, pushing it as [{}, {}]", argument, splitString[0], "");
			g_Arguments[splitString[0]] = "";
		}
		else
		{
			CT_CORE_ERROR("Invalid argument passed into ArgumentHandler::PushArgument()!");
			//g_Arguments[splitString[0]] = splitString[1];
		}
	}
	bool HasArgument(const std::string& arg)
	{
		return g_Arguments.contains(arg);
	}
	std::string GetArgumentValue(const std::string& arg)
	{
		//if (!HasArgument(arg)) return "";
		return g_Arguments[arg];
	}

	static const char* g_ArgFilePath;

	void PushFilePath(const char* filePath)
	{
		g_ArgFilePath = filePath;
	}

	const char* GetFilePath()
	{
		return g_ArgFilePath;
	}
}
