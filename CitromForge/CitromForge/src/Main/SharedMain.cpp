#include "SharedMain.h"

#include "../Tests/TestMain.h"
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

using namespace Citrom;

// Global variables
Platform::Window g_Window;

LayerStack g_LayerStack;
ImGuiLayer g_ImLayer;

int SharedMain(int argc, char* argv[])
{
	CT_INFO("Command Line Arguments ({}):", argc);
	for (int i = 0; i < argc; i++)
	{
		CT_INFO("\t{}: {}", i+1, argv[i]);
	}

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

	using namespace Platform;

	g_Window.Create(1280, 720, CTL::String("test"));

	// TODO: temporary
	Renderer::Initialize();

	ImGuiLayer imLayer;
	imLayer.OnAttach();
	imLayer.Initialize(&g_Window);

	// TODO: Layer System
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

		g_Window.PollEvents();

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

#ifdef CT_PLATFORM_WINDOWS
	std::cin.get();
#endif

	return 0;
}
