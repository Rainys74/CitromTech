#pragma once

#include "Core.h"
#include "Platform/PlatformConsole.h"

#include <string>
#include <string_view>
#include <format>

#include <chrono>
#include <ctime>
#include <iomanip>

/*
* Old Logger:
	Info = variables/information - Gray
	Log = something somewhat important got executed - Green
	Warn = something that shouldn't happen but the process can still run - Yellow
	Error = something that shouldn't happen but the application can still run - Purple
	Fatal/Critical/Severe = something that shouldn't happen and the application must close or already has - Red

* New Logger:
	Trace = track the program execution - Blue (most likely dark)
	//Debug/Verbose = similar to Trace, except for information instead of execution, Example: current cmd line arguments - Green
	Verbose/Debug = similar to Trace, except happens less often and is only used for more important things - Green
	Info/Diag(nostic) = important information that is supposed to happen under normal conditions - Gray
	Warn = something that shouldn't happen but the process can still run - Yellow
	Error = something that shouldn't happen but the application can still run - Purple
	Fatal/Severe/Critical = something that shouldn't happen and the application must close or already has - Red
*/

#ifdef CT_COMPILER_MSVC
#define INTERNAL__VA_OPT__(x) x
#else
#define INTERNAL__VA_OPT__(x) __VA_OPT__(x)
#endif

// typeid(*this).name()
#define CT_CORE_TRACE(x, ...)		Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Trace, "{}(): " x, __func__, ## __VA_ARGS__)
#define CT_CORE_VERBOSE(x, ...)		Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Debug, x, ## __VA_ARGS__)
#define CT_CORE_INFO(x, ...)		Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Info, x, ## __VA_ARGS__)
#define CT_CORE_WARN(x, ...)		Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Warn, x, ## __VA_ARGS__)
#define CT_CORE_ERROR(x, ...)		Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Error, x, ## __VA_ARGS__)
#define CT_CORE_FATAL(x, ...)		Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Critical, x, ## __VA_ARGS__)

//#define CT_TRACE(x, ...)			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::App,	Citrom::Logger::LogLevel::Trace, "{}({}): " ## x, __func__, __LINE__, __VA_ARGS__)
#define CT_TRACE(x, ...)			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::App,	Citrom::Logger::LogLevel::Trace, "{}(): " x, __func__, ## __VA_ARGS__)
#define CT_VERBOSE(x, ...)			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::App,	Citrom::Logger::LogLevel::Debug, x, ## __VA_ARGS__)
#define CT_INFO(x, ...)				Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::App,	Citrom::Logger::LogLevel::Info, x, ## __VA_ARGS__)
#define CT_WARN(x, ...)				Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::App,	Citrom::Logger::LogLevel::Warn, x, ## __VA_ARGS__)
#define CT_ERROR(x, ...)			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::App,	Citrom::Logger::LogLevel::Error, x, ## __VA_ARGS__)
#define CT_FATAL(x, ...)			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::App,	Citrom::Logger::LogLevel::Critical, x, ## __VA_ARGS__)

namespace Citrom
{
	enum class LogColor
	{
		Reset,
		Red,
		Green,
		Yellow,
		Blue,
		Purple
	};

	class Logger
	{
	public:
		enum class LogCategory 
		{
			Core,
			App
		};
		enum class LogLevel 
		{
			Trace,
			Debug, // also known as Verbose
			Info, // might be used for Diagnostics
			Warn,
			Error,
			Critical // also known as Severe/Fatal
		};

		static Logger* GetLogger();

		// Variadic Templates are better and faster than C-style va_lists
		template<typename... Args>
		void Log(const LogCategory category, const LogLevel level, const char* fmt, Args&&... args);
	private:
		Logger();
	};

	template<typename... Args>
	void Logger::Log(const Logger::LogCategory category, const Logger::LogLevel level, const char* fmt, Args&&... args)
	{
		// Format log and arguments
		std::string_view strV(fmt);
		//std::string formattedLog = std::vformat(strV, std::make_format_args(std::forward<Args>(args)...)); // works on windows
		// Create an lvalue tuple of arguments
    	auto formatArgs = std::make_format_args(args...);
		std::string formattedLog = std::vformat(strV, formatArgs);

		// Format TimeStamp
		std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); // or use time(NULL)
		std::tm* timeInfo = std::localtime(&currentTime);

		// Format the time with leading zeros
		//std::stringstream formattedTimeSStream;
		//formattedTimeSStream << "[" << std::setfill('0') << std::setw(2) << timeInfo->tm_hour << ":" << std::setfill('0') << std::setw(2) << timeInfo->tm_min << ":" << std::setfill('0') << std::setw(2) << timeInfo->tm_sec << "]: ";
		//formattedTimeSStream.str();

		char formattedTime[13] = "[00:00:00]: ";
		std::snprintf(formattedTime, 13, "[%02d:%02d:%02d]: ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

		// Get PreMessage
		/*char* preMessage;
		switch (level)
		{
			#define SET_STRING_BY_ENUM(VALUETOSET, ENUMCLASSPATH, ENUM) case ENUMCLASSPATH ## :: ## ENUM:				\
																			VALUETOSET = #ENUM;							\
																			break							
			default: preMessage = "Unknown"; break;
			//SET_STRING_BY_ENUM(preMessage, LogLevel, Trace);
			//SET_STRING_BY_ENUM(preMessage, LogLevel, Info);
			//SET_STRING_BY_ENUM(preMessage, LogLevel, Debug);
			//SET_STRING_BY_ENUM(preMessage, LogLevel, Warn);
			//SET_STRING_BY_ENUM(preMessage, LogLevel, Error);
			//SET_STRING_BY_ENUM(preMessage, LogLevel, Critical);

			SET_STRING_BY_ENUM(preMessage, LogCategory, Core);
			SET_STRING_BY_ENUM(preMessage, LogCategory, App);
		}*/

		switch (level)
		{
			default:					Platform::Console::SetTextColor(Platform::Console::TextColor::Reset); break;
			case LogLevel::Trace:		Platform::Console::SetTextColor(Platform::Console::TextColor::Blue); break;
			case LogLevel::Debug:		Platform::Console::SetTextColor(Platform::Console::TextColor::Green); break;
			case LogLevel::Info:		Platform::Console::SetTextColor(Platform::Console::TextColor::Reset); break;
			case LogLevel::Warn:		Platform::Console::SetTextColor(Platform::Console::TextColor::Yellow); break;
			case LogLevel::Error:		Platform::Console::SetTextColor(Platform::Console::TextColor::Purple); break;
			case LogLevel::Critical:	Platform::Console::SetTextColor(Platform::Console::TextColor::Red); break;
		}

		std::ostringstream result;
		result << formattedTime << formattedLog << '\n';

		Platform::Console::PrintText(result.str().c_str(), (level >= LogLevel::Error) ? Platform::Console::Stream::Error : Platform::Console::Stream::In);
		Platform::Console::SetTextColor(Platform::Console::TextColor::Reset);
	}
}