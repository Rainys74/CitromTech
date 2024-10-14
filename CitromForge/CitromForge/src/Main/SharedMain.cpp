#include "SharedMain.h"

#include "test.h"

int SharedMain(int argc, char* argv[])
{
	Test::PrintSomeShit("insert some random shit!");
	Test::TestOutGLFW();

	return 0;
}
