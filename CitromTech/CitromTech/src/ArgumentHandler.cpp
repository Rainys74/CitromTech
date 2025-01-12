#include "ArgumentHandler.h"
#include "CitromAssert.h"

#include "CTL/HashMap.h"

#include <fstream>

namespace Citrom::ArgumentHandler
{
	static CTL::HashMap<std::string, std::string> g_Arguments;

	CTL::DArray<std::string> GetArgumentsFromFile(const std::string& filePath)
	{
		CTL::DArray<std::string> args;

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
	}
	bool HasArgument(const std::string& arg)
	{
		return false;
	}
	std::string GetArgumentValue(const std::string& arg)
	{
		return std::string();
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