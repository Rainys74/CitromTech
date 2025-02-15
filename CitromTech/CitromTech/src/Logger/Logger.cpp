#include "Logger.h"

namespace Citrom
{
    Logger* Logger::GetLogger()
    {
        static Logger instance;
		return &instance;
	}
    void Logger::PushCallback(LoggerCallbackFN callback)
    {
        m_Callbacks.PushBack(callback);
    }

    Logger::Logger()
    {
        // Console logging should only happen on Debug builds.
        #ifdef CT_DEBUG
        Platform::Console::CreateConsole();
        m_Callbacks.PushBack([](const char* log, LogColor logColor, LogLevel level)
        {
            Platform::Console::SetTextColor(static_cast<Platform::Console::TextColor>(logColor));
            Platform::Console::PrintText(log, (level >= LogLevel::Error) ? Platform::Console::Stream::Error : Platform::Console::Stream::In);
            Platform::Console::SetTextColor(Platform::Console::TextColor::Reset);
        });
        #endif
    }
}
