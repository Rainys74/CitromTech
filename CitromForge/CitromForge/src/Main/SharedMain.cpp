#include "SharedMain.h"

#include "test.h"
#include "CTL/String.h"
#include "CTL/StaticArray.h"
#include "CTL/DArray.h"
#include <string>
#include "Logger/Logger.h"
#include "Platform/PlatformConsole.h"
#include "CitromAssert.h"

#include "Events/EventSystem.h"
#include "Events/KeyEvents.h"
#include "Events/WindowEvents.h"

#include "Platform/PlatformWindow.h"

#include "Audio/AudioSystem.h"

#include <iostream>

using namespace Citrom;

int SharedMain(int argc, char* argv[])
{
	Audio::Initialize();

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

	KeyDownEvent keyDownEvent(1254);

	CT_TRACE("Event Type: {}", (int)keyDownEvent.GetEventType());

	CT_TRACE("Event Category Name: {}", keyDownEvent.GetEventCategoryName());
	CT_TRACE("Event Type Name: {}", keyDownEvent.GetEventTypeName());
	CT_TRACE("Event To String: {}", keyDownEvent.ToString().CStr());

	EventListener<KeyEvents> keyEventListener;
	keyEventListener.OnEvent = [](const Event<KeyEvents>& event) {
		CT_ERROR("TEST!: {}", (int)event.GetEventType());

		CT_VERBOSE("Event Category Name: {}", event.GetEventCategoryName());
		CT_VERBOSE("Event Type Name: {}", event.GetEventTypeName());
		CT_TRACE("Event To String: {}", event.ToString().CStr());
	};

	EventDispatcher<KeyEvents> keyEventDispatcher;
	keyEventDispatcher.AddListener(&keyEventListener);
	keyEventDispatcher.Dispatch(keyDownEvent);
	
	EventBusOld eventBus;
	eventBus.AddListener<KeyEvents>(&keyEventListener);
	eventBus.Dispatch<KeyEvents>(keyDownEvent);

	EventListener<WindowEvents> windowEventListener;
	windowEventListener.OnEvent = [](const Event<WindowEvents>& event) {
		CT_ERROR("Window Event!: {}", (int)event.GetEventType());

		CT_VERBOSE("Event Category Name: {}", event.GetEventCategoryName());
		CT_VERBOSE("Event Type Name: {}", event.GetEventTypeName());
		CT_TRACE("Event To String: {}", event.ToString().CStr());

		if (event.GetEventType() == WindowEvents::WindowResize)
		{
			const WindowResizeEvent& transformedEvent = (const WindowResizeEvent&)event;
			CT_TRACE("Event Width: {}", transformedEvent.width);
			CT_TRACE("Event Height: {}", transformedEvent.height);
		}
	};

	Audio::Clip clip;
	clip.path = FileSystem::FilePath("E:/Citrom Tech/bin/Debug-windows-x86_64/CitromForge/anothersound.wav");
	clip.volume = 1.0f;
	clip.looping = true;
	Audio::PlayAudioClip(&clip);

	//EventBus::GetInstance()->AddListener<WindowEvents>(&windowEventListener);
	EventBus::GetDispatcher<WindowEvents>()->AddListener(&windowEventListener);

	int a = 5;
	float b = 2.5334f;
	const char* c = "this is a test";

	CTL::DArray<int> arrayTest(2);
	arrayTest.PushBack(4);
	arrayTest.PushBack(2);
	arrayTest.PushBack(7);
	arrayTest.PushBack(8);

	for (auto item : arrayTest)
	{
		CT_VERBOSE("{}", item);
	}

	arrayTest[2] = 90;

	for (auto item : arrayTest)
	{
		CT_VERBOSE("{}", item);
	}

	//Logger::GetLogger()->Log(Logger::LogCategory::App, Logger::LogLevel::Info, "A = {}, B = {}, C = {}", a, b, c);
	CT_TRACE("A = {}, B = {}, C = {}", a, b, c);

	std::string string3("This ");

	string3.append("is an 👋 ");
	string3.append("🌍");
	string3.append(" test!");

	// Create an UnorderedMap using xxHash for int keys
	CTL::HashMap<int, std::string> intMap;
	intMap[1] = "one";
	intMap[2] = "two";
	intMap[3] = "Three";
	intMap[4] = "four";
	intMap[5] = "FIve";

	for (const auto& pair : intMap) 
	{
		CT_TRACE("IntMap: pair.first: {}, pair.second: {}", pair.first, pair.second);
	}

	Test::PrintSomeShit(const_cast<char*>(string3.c_str()));

	using namespace Platform;
	Window window;

	window.Create(1280, 720, CTL::String("test"));

	while (!window.WindowShouldClose())
	{
		window.PollEvents();
	}

	//Test::TestOutGLFW();

	Test::PrintSomeShit(const_cast<char*>("this is an error test"));
	//DEBUG_BREAK();
	Test::PrintSomeShit(const_cast<char*>("this is an error test after the debug break"));

#ifdef CT_PLATFORM_WINDOWS
	std::cin.get();
#endif

	return 0;
}
