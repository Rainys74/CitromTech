#pragma once

#include "Core.h"

#include <string>
#include <string_view>
#include <format>
#include <iostream>

#include <chrono>
#include <ctime>
#include <iomanip>

// Config
#define C_STYLE_TIME

/*
* Old Logger:
	Info = variables/information - Gray
	Log = something somewhat important got executed - Green
	Warn = something that shouldn't happen but the process can still run - Yellow
	Error = something that shouldn't happen but the application can still run - Purple
	Fatal/Critical/Severe = something that shouldn't happen and the application must close or already has - Red

* New Logger:
	Trace = track the program execution - Blue (most likely dark)
	Info = important information that is supposed to happen under normal conditions - Gray
	Debug = something - maybe green?
	Warn = something that shouldn't happen but the process can still run - Yellow
	Error = something that shouldn't happen but the application can still run - Purple
	Fatal/Severe/Critical = something that shouldn't happen and the application must close or already has - Red
*/
#define CT_TRACE(x, ...)	Logger::GetLogger()->Log(Logger::LogCategory::App, Logger::LogLevel::Trace, x, __VA_ARGS__);

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
			Info,
			Debug,
			Warn,
			Error,
			Critical // also known as Severe/Fatal
		};

		static Logger* GetLogger();

		// Variadic Templates are better and faster than C-style va_lists
		template<typename... Args>
		void Log(const LogCategory category, const LogLevel level, const char* fmt, Args&&... args);
	private:
		// static logger instances
	};

	template<typename... Args>
	void Logger::Log(const Logger::LogCategory category, const Logger::LogLevel level, const char* fmt, Args&&... args)
	{
		// Format log and arguments
		std::string_view strV(fmt);
		std::string formattedLog = std::vformat(strV, std::make_format_args(std::forward<Args>(args)...));

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

		std::ostringstream result;
		result << formattedTime << formattedLog;

		std::cout << result.str().c_str() << '\n';
	}
}