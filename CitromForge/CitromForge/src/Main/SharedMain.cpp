#include "SharedMain.h"

#include "Tests/TestMain.h"
#include "CTL/String.h"
#include "CTL/StaticArray.h"
#include "CTL/DArray.h"
#include <string>
#include "Logger/Logger.h"
#include "Platform/PlatformConsole.h"
#include "CitromAssert.h"
#include "ArgumentHandler.h"

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

#include "Application/EditorLayer.h"

#include <iostream>

using namespace Citrom;

// Global variables
Platform::Window g_Window;

LayerStack g_LayerStack;
ImGuiLayer g_ImLayer;

float64 g_DeltaTime; // Frame Time

// Layers
EditorLayer g_EditorLayer;

float64 MainDeltaTime()
{
	return g_DeltaTime;
}

float64 MainFPS()
{
	return 1.0 / g_DeltaTime;
}

int SharedMain(int argc, char* argv[])
{
	CT_INFO("Command Line Arguments ({}):", argc);
	for (int i = 0; i < argc; i++)
	{
		CT_INFO("\t{}: {}", i+1, argv[i]);
	}

	// Push Command line arguments
	ArgumentHandler::PushFilePath(argv[0]);
	for (int i = 0; i < argc; i++)
	{
		ArgumentHandler::PushArgument(argv[i]);
	}

	CTL::DArray<std::string> testArgs = ArgumentHandler::GetArgumentsFromFile("commandline.txt");
	for (const auto& testArg : testArgs)
	{
		ArgumentHandler::PushArgument(testArg.c_str());
	}

	// Initialize Thread Pool
	uint32 threadCount;
	if (ArgumentHandler::HasArgument("-threadCount"))
	{
		std::string argValue = ArgumentHandler::GetArgumentValue("-threadCount");

		if (argValue.find("%Cores"))
		{
			threadCount = Platform::Info::GetNumberOfProcessors();
		}
		else if (argValue.find("%Threads"))
		{
			threadCount = Platform::Info::GetNumberOfLogicalProcessors();
		}
		else
		{
			threadCount = std::stoul(argValue);
		}
	}
	else
	{
		threadCount = Platform::Info::GetNumberOfLogicalProcessors();
	}

	// Checks for Thread Count
	if (threadCount < 3)
	{
		CT_WARN("Thread count was set to less than 3! Setting it to 3.");
		threadCount = 3;
	}
	if (threadCount < 5)
		CT_WARN("Thread count was set to ({}), Minimum Recommended value: {}", threadCount, 5);
	if (threadCount > 0xFFFFFFFF)
	{
		CT_WARN("Thread count was set higher than the max uint32 value! Clamping it now!");
		threadCount = 0xFFFFFFFF;
	}
	if (threadCount > 128)
		CT_WARN("Thread count was set to ({}), Make sure it was intentional to set Thread Count higher than 128!");

	CT_INFO("Initializing Thread Pool with {} threads. Thread count is ({})", threadCount - 1, threadCount);

	// TODO: eventually change the -1
	ThreadPool::Initialize(threadCount - 1);

#if 0
	TestOutMain(argc, argv);
#endif

	EventListener<KeyEvents> keyEventListener;
	keyEventListener.OnEvent = [](const Event<KeyEvents>& event) {
		CT_ERROR("TEST!: {}", (int)event.GetEventType());

		CT_VERBOSE("Event Category Name: {}", event.GetEventCategoryName());
		CT_VERBOSE("Event Type Name: {}", event.GetEventTypeName());
		CT_TRACE("Event To String: {}", event.ToString().CStr());
	};

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

	EventBus::GetDispatcher<WindowEvents>()->AddListener(&windowEventListener);
	EventBus::GetDispatcher<MouseEvents>()->AddListener(&mouseEventListener);

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

	CT_WARN("Threads: {}", Platform::Info::GetNumberOfLogicalProcessors());
	CT_WARN("Cores: {}", Platform::Info::GetNumberOfProcessors());
	CT_WARN("Platform: {}", Platform::Info::GetPlatformName());
	CT_WARN("OS Info: {}", Platform::Info::GetOSInfo());

	CT_WARN("Build Type: [{}]", GetBuildType());

	CT_WARN("Editor Build: {}", IsEditorBuild());

	//CT_CORE_ASSERT(false, "Test assert!");

	using namespace Platform;

	g_Window.Create(1280, 720, CTL::String("test"));

	// TODO: temporary
	Renderer::Initialize(&g_Window);

	g_ImLayer.OnAttach();
	g_ImLayer.Initialize(&g_Window);

	// Push Layers
	g_LayerStack.Push(&g_EditorLayer);

	::ForgeLoop();

	g_ImLayer.OnDetach();

	//g_Window.~Window(); // Somehow throws an assertion during "delete m_Backend;" the assertion being because m_Backend gets deleted twice...

#ifdef CT_PLATFORM_WINDOWS
	std::cin.get();
#endif

	return 0;
}

void ForgeLoop()
{
	float64 currentTime; // New Time
	float64 previousTime = Platform::Utils::GetTime(); // Old Time

	while (!g_Window.WindowShouldClose())
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

		currentTime = Platform::Utils::GetTime();
		g_DeltaTime = currentTime - previousTime;
		previousTime = currentTime;
		//float64 fps = 1.0 / frameTime;
		//CT_WARN("FPS: {} (Frame/Delta Time: {} ({} ms))", fps, frameTime, frameTime * 1000);

		g_Window.PollEvents();

		// Update & Tick (Fixed Update)
		g_LayerStack.Update();

		// while (accumulated time >= fixed time step) probably

		// Render
		Renderer::BeginFrame();
		g_LayerStack.Render();

		{
			CT_PROFILE_SCOPE("ImGui Render");
			g_ImLayer.Begin();
			g_LayerStack.ImGuiRender();
			g_ImLayer.End();
		}

		Renderer::EndFrame();

		/*Profiler::ProfileResults::IterateResultsCallback([](const char* key, const float64 time)
		{
			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Trace, "{}(): " "Profiling {} took {} ms!!!!", __func__, key, time * 1000);
		});*/

		//Profiler::ProfileResults::PrintResults();
		//CT_WARN("{}", Profiler::ProfileResults::RetrieveTime("class Citrom::Platform::Window::PollEvents()") * 1000);
	}
}