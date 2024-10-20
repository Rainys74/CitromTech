#include "Logger.h"

namespace Citrom
{
    Logger* Logger::GetLogger()
    {
        static Logger instance;
		return &instance;
	}

    template <typename... Args>
    void Logger::Log(const LogCategory category, const Args &&...args)
    {

    }
}