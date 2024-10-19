#include "SharedMain.h"

#include "test.h"
#include "CTL/String.h"

int SharedMain(int argc, char* argv[])
{
	Test::PrintSomeShit("insert some random shit!");

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

	Test::TestOutGLFW();

	Test::PrintSomeShit("this is an error test");
	//DEBUG_BREAK();
	Test::PrintSomeShit("this is an error test after the debug break");

	return 0;
}
