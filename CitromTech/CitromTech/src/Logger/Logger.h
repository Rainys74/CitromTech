#pragma once

#include "Core.h"

//#include <format>

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
#define CT_TRACE(...)	//__VA_ARGS__

namespace Citrom
{
	class Logger
	{
	public:
		enum class LogCategory 
		{
			Core,
			App
		};

		static Logger* GetLogger();

		// Variadic Templates are better and faster than C-style va_lists
		template<typename... Args>
		void Log(const LogCategory category, const Args&&... args);
	private:
		// static logger instances
	};
}