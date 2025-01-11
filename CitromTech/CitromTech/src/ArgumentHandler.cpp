#include "ArgumentHandler.h"

#include "CTL/HashMap.h"

namespace Citrom::ArgumentHandler
{
	static CTL::HashMap<std::string, std::string> g_Arguments;

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