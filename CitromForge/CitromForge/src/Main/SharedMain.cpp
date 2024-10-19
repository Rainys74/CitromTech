#include "SharedMain.h"

#include "test.h"

int SharedMain(int argc, char* argv[])
{
	Test::PrintSomeShit("insert some random shit!");

	Test::PrintComplexEmoji("👋, 🌍!");
	Test::PrintComplexEmoji("Ką jūs, pasaulį!");

	Test::TestOutGLFW();

	Test::PrintSomeShit("this is an error test");
	//DEBUG_BREAK();
	Test::PrintSomeShit("this is an error test after the debug break");

	return 0;
}
