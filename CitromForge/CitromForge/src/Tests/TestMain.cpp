#include "TestMain.h"

#include "test.h"
#include "CTL/String.h"
#include "CTL/StaticArray.h"
#include "CTL/DArray.h"
#include "CTL/HashMap.h"
#include <string>
#include "Logger/Logger.h"
#include "Platform/PlatformConsole.h"
#include "CitromAssert.h"

#include "Events/EventSystem.h"
#include "Events/KeyEvents.h"
#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"

#include "Platform/PlatformWindow.h"
#include "Platform/Platform.h"

#include "Renderer/Renderer.h"

#include "Math/MathCommon.h"
#include "Math/Vector.h"

#include "Profiling/Profiler.h"

#include "ThreadPool.h"
#include "LayerSystem/ImGuiLayer.h"

#include <iostream>

// ImguI temp
#include "Platform/PlatformImGui.h"

using namespace Citrom;

uint32 tpvar = 0;
void ThreadPoolTestJob()
{
	for (uint32 i = 0; i < 1000; i++)
	{
		tpvar++;
	}
}

void TestOutMain(int argc, char** argv)
{
	CT_INFO("Command Line Arguments ({}):", argc);
	for (int i = 0; i < argc; i++)
	{
		CT_INFO("\t{}: {}", i + 1, argv[i]);
	}

	CT_TRACE("Test {}", (int)5);

	CT_ASSERT(1 == 1, "1 != 0");

	//Test::TestOutGLSLCC(argv[0]); //TODO: implement dll loading on unix

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

	KeyDownEvent keyDownEvent; //(1254);

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

	EventListener<MouseEvents> mouseEventListener;
	mouseEventListener.OnEvent = [](const Event<MouseEvents>& event) {
		CT_ERROR("Mouse Event!: {}", (int)event.GetEventType());

		CT_VERBOSE("Event Category Name: {}", event.GetEventCategoryName());
		CT_VERBOSE("Event Type Name: {}", event.GetEventTypeName());
		CT_TRACE("Event To String: {}", event.ToString().CStr());

		if (event.GetEventType() == MouseEvents::MouseDown)
		{
			const MouseDownEvent& transformedEvent = (const MouseDownEvent&)event;
			CT_TRACE("Mouse Button: {}", (int)transformedEvent.mouseButton);
		}
		else if (event.GetEventType() == MouseEvents::MouseUp)
		{
			const MouseUpEvent& transformedEvent = (const MouseUpEvent&)event;
			CT_TRACE("Mouse Button: {}", (int)transformedEvent.mouseButton);
		}
		};

	//Audio::Clip clip;
	//clip.path = FileSystem::FilePath("E:/Citrom Tech/bin/Debug-windows-x86_64/CitromForge/anothersound.wav");
	//clip.volume = 1.0f;
	//clip.looping = true;
	//Audio::PlayAudioClip(&clip);

	//EventBus::GetInstance()->AddListener<WindowEvents>(&windowEventListener);
	EventBus::GetDispatcher<WindowEvents>()->AddListener(&windowEventListener);
	EventBus::GetDispatcher<MouseEvents>()->AddListener(&mouseEventListener);

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

	CT_WARN("Real Inverse Square Root: {}", (float32)(1 / Math::SquareRoot(5.0f)));
	CT_WARN("Q3Ap Inverse Square Root: {}", (float32)Math::InverseSquareRoot(5.0f));

	//Math::Vector<3> vector;
	Math::Vector3 vector;

	vector[0] = 4.0f;
	vector[1] = 5.7f;
	vector[2] = 7.482f;

	CT_VERBOSE("Vector3: ({}, {}, {})", vector[0], vector[1], vector[2]);

	CT_VERBOSE("Magnitude: {}", vector.Magnitude());
	CT_VERBOSE("Squared Magnitude: {}", vector.SquaredMagnitude());
	CT_VERBOSE("Normalized: ({}, {}, {})", vector.Normalized()[0], vector.Normalized()[1], vector.Normalized()[2]);

	//CT_VERBOSE("Dot: {}", vector.Magnitude());
	//CT_VERBOSE("Cross: ({}, {}, {})", vector.Magnitude());

	// Thread pool
	ThreadPool threadPool(Platform::Info::GetNumberOfLogicalProcessors() - 1);

	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);
	threadPool.Submit((void*)ThreadPoolTestJob, nullptr);
	CT_INFO("tpvar: {}", tpvar);


	CT_WARN("Threads: {}", Platform::Info::GetNumberOfLogicalProcessors());
	CT_WARN("Cores: {}", Platform::Info::GetNumberOfProcessors());
	CT_WARN("Platform: {}", Platform::Info::GetPlatformName());
	CT_WARN("OS Info: {}", Platform::Info::GetOSInfo());

	CT_ERROR("CT_PRETTY_FUNCTION: {}", CT_PRETTY_FUNCTION);
	//CT_ERROR("__FUNCSIG__: {}", __FUNCSIG__);
	//CT_ERROR("__PRETTY_FUNCTION__: {}", __PRETTY_FUNCTION__);

	using namespace Platform;
	Window window;

	window.Create(1280, 720, CTL::String("test"));

	// Dear ImGui
#if 0
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	InitializeImGuiWindow();

	ImGui::StyleColorsDark();
#endif

	// TODO: why doesn't work??!
	//Platform::ImGui::Initialize(&window);

	// TODO: temporary
	Renderer::Initialize(&window);

	ImGuiLayer imLayer;
	imLayer.OnAttach();
	imLayer.Initialize(&window);

	// TODO: Layer System
	while (!window.WindowShouldClose())
	{
		// TODO: Timing. Choose one. (Though constructing multiple doubles every frame might be idiotic)
		// 1
		//float64 newTime = platform_get_time();
		//sg_FrameTime = newTime - currentTime;
		//currentTime = newTime;
		//float64 fps = 1.0 / sg_FrameTime;

		/// VS

		// 2
		//float64 startTime = Platform::Utils::GetTime();
		//// Loop
		//float64 endTime = Platform::Utils::GetTime();
		//float64 frameTime = endTime - startTime;
		//float64 fps = 1.0 / frameTime;

		window.PollEvents();

		// Update & Tick (Fixed Update)

		// Render
		imLayer.Begin();
		// Layers->ImGuiRender()
		imLayer.End();

		/*Profiler::ProfileResults::IterateResultsCallback([](const char* key, const float64 time)
		{
			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Trace, "{}(): " "Profiling {} took {} ms!!!!", __func__, key, time * 1000);
		});*/

		//Profiler::ProfileResults::PrintResults();
		//CT_WARN("{}", Profiler::ProfileResults::RetrieveTime("class Citrom::Platform::Window::PollEvents()") * 1000);
	}

	imLayer.OnDetach();

	//Test::TestOutGLFW();

	Test::PrintSomeShit(const_cast<char*>("this is an error test"));
	//DEBUG_BREAK();
	Test::PrintSomeShit(const_cast<char*>("this is an error test after the debug break"));

#ifdef CT_PLATFORM_WINDOWS
	std::cin.get();
#endif
}
