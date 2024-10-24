#include "Logger.h"

namespace Citrom
{
    Logger* Logger::GetLogger()
    {
        static Logger instance;
		return &instance;
	}
    Logger::Logger()
    {
        Platform::Console::CreateConsole();
    }
}