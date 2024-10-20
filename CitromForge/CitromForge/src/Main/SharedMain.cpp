#include "SharedMain.h"

#include "test.h"
#include "CTL/String.h"
#include <string>
#include "Logger/Logger.h"

using namespace Citrom;

int SharedMain(int argc, char* argv[])
{
	Test::PrintSomeShit(const_cast<char*>("insert some random shit!"));

	Test::PrintComplexEmoji("👋, 🌍!");
	Test::PrintComplexEmoji("Ką jūs, pasaulį!");

	CTL::String string1;

	string1.PushBack('T');
	string1.Append("his is string 1!");

	Test::PrintSomeShit(string1.CStr());

	CTL::String string2("Hello, ");
	string2.Append("W");
	string2.PushBack('o');
	string2.Append("rld!");

	Test::PrintSomeShit(string2.CStr());

	int a = 5;
	float b = 2.5334f;
	const char* c = "this is a test";

	//Logger::GetLogger()->Log(Logger::LogCategory::App, Logger::LogLevel::Info, "A = {}, B = {}, C = {}", a, b, c);
	CT_TRACE("A = {}, B = {}, C = {}", a, b, c);

	std::string string3("This ");

	string3.append("is an 👋 ");
	string3.append("🌍");
	string3.append(" test!");

	Test::PrintSomeShit(const_cast<char*>(string3.c_str()));

	Test::TestOutGLFW();

	Test::PrintSomeShit(const_cast<char*>("this is an error test"));
	//DEBUG_BREAK();
	Test::PrintSomeShit(const_cast<char*>("this is an error test after the debug break"));

	return 0;
}
