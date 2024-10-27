#include "SharedMain.h"

#include "test.h"
#include "CTL/String.h"
#include "CTL/StaticArray.h"
#include <string>
#include "Logger/Logger.h"
#include "Platform/PlatformConsole.h"
#include "CitromAssert.h"

#include "Events/EventSystem.h"

#include <iostream>

using namespace Citrom;

int SharedMain(int argc, char* argv[])
{
	CT_INFO("Command Line Arguments ({}):", argc);
	for (int i = 0; i < argc; i++)
	{
		CT_INFO("\t{}: {}", i+1, argv[i]);
	}

	CT_TRACE("Test {}", (int)5);

	CT_ASSERT(1 == 1, "1 != 0");

	Test::PrintSomeShit(const_cast<char*>("insert some random shit!"));

	Test::PrintComplexEmoji("👋, 🌍!");
	Test::PrintComplexEmoji("Ką jūs, pasaulį!");

	CTL::Array<char, 8> testSArray;
	testSArray[0] = 't';
	testSArray[1] = 'e';
	testSArray[2] = 's';
	testSArray[3] = 't';
	testSArray[4] = 'i';
	testSArray[5] = 'n';
	testSArray[6] = 'g';
	testSArray[7] = '!';

	for (auto letter : testSArray)
	{
		CT_VERBOSE("{}", letter);
	}

	CT_TRACE("Modifying letter..");
	testSArray[7] = '1';
	CT_TRACE("Successfully modified letter.");

	for (auto letter : testSArray)
	{
		CT_VERBOSE("{}", letter);
	}

	CTL::String string1;

	string1.PushBack('T');
	string1.Append("his is string 1!");

	Test::PrintSomeShit(string1.CStr());

	CTL::String string2("Hello, ");
	string2.Append("W");
	string2.PushBack('o');
	string2.Append("rld!");

	Test::PrintSomeShit(string2.CStr());

	Platform::Console::SetTextColor(Platform::Console::TextColor::Blue);
	Platform::Console::PrintText("this is a 👋, 🌍! test!\n", Platform::Console::Stream::Out);		// ansi/utf-8
	Platform::Console::PrintText(L"this is a 👋, 🌍! test!\n", Platform::Console::Stream::Out);		// wide
	Platform::Console::PrintText(u8"this is a 👋, 🌍! test!\n", Platform::Console::Stream::Out);	// utf-8
	Platform::Console::PrintText(u"this is a 👋, 🌍! test!\n", Platform::Console::Stream::Out);		// utf-16

	KeyDownEvent event;

	CT_TRACE("Event Type: {}", (int)event.GetEventType());

	CT_TRACE("Event Category Name: {}", event.GetEventCategoryName());
	CT_TRACE("Event Type Name: {}", event.GetEventTypeName());
	//CT_TRACE("Event To String: {}", event.ToString().CStr());

	EventListener<KeyEvents> keyEventListener;
	keyEventListener.OnEvent = [](const KeyEvents eventType) {
		CT_ERROR("TEST!: {}", (int)eventType);
	};

	EventDispatcher<KeyEvents> keyEventDispatcher;
	keyEventDispatcher.AddListener(&keyEventListener);
	keyEventDispatcher.Dispatch(KeyEvents::KeyRepeat);

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

	std::cin.get();

	return 0;
}
