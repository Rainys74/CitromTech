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

#include "Editor/EditorCamera.h"

#include "Input/KeyboardInput.h"

#include "Platform/PlatformWindow.h"
#include "Platform/Platform.h"

#include "Renderer/Renderer.h"

#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "EntitySystem/Components/ScriptComponents.h"

#include "Math/MathCommon.h"
#include "Math/Vector.h"
#include "Math/Matrix4x4.h"

#include "Profiling/Profiler.h"

#include "ThreadPool.h"
#include "LayerSystem/ImGuiLayer.h"
#include "LayerSystem/SimpleInputLayer.h"
#include "LayerSystem/NativeScriptLayer.h"

#include "JSON/Reader.h"
#include "JSON/Writer.h"

#include "Application/EditorLayer.h"
#include "Application/ApplicationInfo.h"

#include "Vendor/sol2/sol.hpp"
#include "angelscript.h"

#include <iostream>

using namespace Citrom;

// Global variables
Platform::Window g_Window;

LayerStack g_LayerStack;
IF_EDITOR(ImGuiLayer g_ImLayer);

Scene* g_CurrentScene;

float64 g_DeltaTime; // Frame Time

// Layers
IF_EDITOR(EditorLayer g_EditorLayer);

float64 MainDeltaTime()
{
	return g_DeltaTime;
}

float64 MainFPS()
{
	return 1.0 / g_DeltaTime;
}

Scene* GetCurrentScene()
{
	return g_CurrentScene;
}

static Camera g_NullCamera;
static Math::Transform g_NullCameraTransform;

Camera* GetCamera()
{
	//return EditorCamera::Get()->IsActive() ? EditorCamera::Get()->GetCamera() : &g_CurrentScene->GetMainCameraEntity().GetComponent<CameraComponent>().camera;
	if (IF_EDITOR(true) IF_NEDITOR(false) && EditorCamera::Get()->IsActive() && EditorCamera::Get()->GetCamera())
	{
		return EditorCamera::Get()->GetCamera();
	}
	else
	{
		if (g_CurrentScene->GetMainCameraEntity())
		{
			return &g_CurrentScene->GetMainCameraEntity().GetComponent<CameraComponent>().camera;
		}
		else
		{
			g_NullCamera.clearColor = Math::ColorF32x4(1.0f, 0.0f, 1.0f);
			return &g_NullCamera;
		}
	}
}
Math::Transform* GetCameraTransform()
{
	//return EditorCamera::Get()->IsActive() ? EditorCamera::Get()->GetTransform() : &g_CurrentScene->GetMainCameraEntity().GetComponent<TransformComponent>().transform;
	if (IF_EDITOR(true) IF_NEDITOR(false) && EditorCamera::Get()->IsActive() && EditorCamera::Get()->GetTransform())
	{
		return EditorCamera::Get()->GetTransform();
	}
	else
	{
		if (g_CurrentScene->GetMainCameraEntity())
			return &g_CurrentScene->GetMainCameraEntity().GetComponent<TransformComponent>().transform;
		else
			return &g_NullCameraTransform;
	}
}

Platform::Window* GetMainWindow()
{
	return &g_Window;
}

static void AS_Print(std::string& msg)
{
	CT_INFO("{}", msg);
}

static ApplicationInfoSpecification GetApplicationSpecification_Editor()
{
	ApplicationInfoSpecification spec;

	spec.windowInfo.width = PLATFORM_DEFAULT_WIDTH;
	spec.windowInfo.height = PLATFORM_DEFAULT_HEIGHT;
	spec.windowInfo.defaultTitle = std::string("Citrom Forge" " [Editor]") + " [" + GetBuildType() + "]";

	spec.windowInfo.displayMode = Platform::DisplayMode::Windowed;

	spec.rendererInfo.apiPriorityList[0] = RenderAPI::GraphicsAPI::DirectX11;
	spec.rendererInfo.apiPriorityList[1] = RenderAPI::GraphicsAPI::Metal;
	spec.rendererInfo.apiPriorityList[2] = RenderAPI::GraphicsAPI::OpenGL;
	//spec.rendererInfo.defaultVSync = RenderAPI::VSyncMode::On;

	//spec.mainWindow = &g_Window;

	return spec;
}
static ApplicationInfoSpecification GetApplicationSpecification_Runtime()
{
	ApplicationInfoSpecification spec;

	spec.windowInfo.width = PLATFORM_DEFAULT_WIDTH;
	spec.windowInfo.height = PLATFORM_DEFAULT_HEIGHT;
	spec.windowInfo.defaultTitle = STRINGIFY_TOKEN(CT_APP_NAME); // TODO: this doesn't work as expected

	spec.windowInfo.displayMode = Platform::DisplayMode::Borderless;

	spec.rendererInfo.apiPriorityList[0] = RenderAPI::GraphicsAPI::DirectX11;
	spec.rendererInfo.apiPriorityList[1] = RenderAPI::GraphicsAPI::Metal;
	spec.rendererInfo.apiPriorityList[2] = RenderAPI::GraphicsAPI::OpenGL;
	//spec.rendererInfo.defaultVSync = RenderAPI::VSyncMode::On;

	//spec.mainWindow = &g_Window;

	return spec;
}

/*static FORCE_INLINE ApplicationInfoSpecification GetApplicationSpecification()
{
#ifdef CT_EDITOR_ENABLED
	return GetApplicationSpecification_Editor();
#else
	return GetApplicationSpecification_Runtime();
#endif
}*/
#ifdef CT_EDITOR_ENABLED
#define GetApplicationSpecification GetApplicationSpecification_Editor
#else
#define GetApplicationSpecification GetApplicationSpecification_Runtime
#endif

int SharedMain(int argc, char* argv[])
{
	CT_INFO("Command Line Arguments ({}):", argc);
	for (int i = 0; i < argc; i++)
	{
		CT_INFO("\t{}: {}", i + 1, argv[i]);
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

		if (argValue.find("%Cores") != std::string::npos)
		{
			threadCount = Platform::Info::GetNumberOfProcessors();
		}
		else if (argValue.find("%Threads") != std::string::npos)
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
	//ThreadPool::Initialize(threadCount - 1);

	//static uint32 total = 0;
	//
	//CTL::DArray<uint32> testIter(5);
	//for (uint32 i = 0; i < 5; i++)
	//	testIter.PushBack(34);
	//ThreadPool::Get()->ForEach<uint32>(testIter.begin(), testIter.end(), [](uint32* iPtr)
	//	{
	//		uint32 i = *(uint32*)iPtr;
	//
	//		CT_ERROR("TP: {}. {}", i, 2);
	//	});
	//
	//CT_WARN("IS TP BUSY?: {}", ThreadPool::Get()->IsBusy());
	//
	//CT_ERROR("TP TOTAL: {}", total);

#if 0
	TestOutMain(argc, argv);
#endif

	EventListener<KeyEvents> keyEventListener;
	keyEventListener.OnEvent = [](const Event<KeyEvents>& event) {
		CT_ERROR("TEST!: {}", (int)event.GetEventType());

		CT_VERBOSE("Event Category Name: {}", event.GetEventCategoryName());
		CT_VERBOSE("Event Type Name: {}", event.GetEventTypeName());
		CT_TRACE("Event To String: {}", event.ToString().CStr());

		const KeyEventBase& transformedEvent = (const KeyEventBase&)event;
		CT_TRACE("KeyCode String: {} ({})", Input::KeyCodeToString(transformedEvent.keyCode), (uint32)transformedEvent.keyCode);
		};

	static EventListener<WindowEvents> windowEventListenerTest;
	windowEventListenerTest.OnEvent = [](const Event<WindowEvents>& event) {
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
		if (event.GetEventType() == MouseEvents::MouseMove)
			return;

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

	EventBus::GetDispatcher<WindowEvents>()->AddListener(&windowEventListenerTest);
	EventBus::GetDispatcher<MouseEvents>()->AddListener(&mouseEventListener);
	EventBus::GetDispatcher<KeyEvents>()->AddListener(&keyEventListener);

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

	//Math::Matrix4x4 testMat(1.0f);
	//Math::Matrix4x4 testMat{ {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4} , {1, 2, 3, 4} };
	Math::Matrix4x4 testMat{ {1, 3, 5, 7}, {2, 4, 6, 8}, {9, 11, 13, 15}, {10, 12, 14, 16} };
	Math::Matrix4x4 idMat(Math::Matrix4x4::Identity());
	CT_VERBOSE("\n{}\n", idMat.ToString());

	CT_WARN("\n{}", testMat.ToString());
	CT_ERROR("Transposing...");
	testMat.Transpose();
	CT_WARN("\n{}", testMat.ToString());

	// Camera Resize System
	static EventListener<WindowEvents> windowEventListener;
	windowEventListener.OnEvent = [](const Event<WindowEvents>& event)
	{
		if (event.GetEventType() == WindowEvents::WindowResize)
		{
			const WindowResizeEvent& transformedEvent = (const WindowResizeEvent&)event;
			
			GetCamera()->SetProjectionType(GetCamera()->GetProjectionType()); // Current workaround for resizing
		}
	};
	EventBus::GetDispatcher<WindowEvents>()->AddListener(&windowEventListener);

	Scene scene; g_CurrentScene = &scene;

	Entity cube1 = g_CurrentScene->CreateEntity();
	Entity e2 = g_CurrentScene->CreateEntity();
	CubeComponent& cubeComponent = cube1.AddComponent<CubeComponent>();
	//cubeComponent.material.shaderName = "Standard";
	Entity camera = g_CurrentScene->CreateEntity();
	camera.GetComponent<NameComponent>().name = "Main Camera";
	camera.GetComponent<TransformComponent>().transform.position.z = -0.5f;

	// is this a valid workaround? No it's not.
	//CameraViewport::Get()->SetViewport(g_Window.GetBackend()->GetWidth(), g_Window.GetBackend()->GetHeight());

	CameraComponent& mainCameraComponent = camera.AddComponent<CameraComponent>();
	mainCameraComponent.camera.SetPerspective(Math::DegreesToRadians(90.0f), 0.01f, 1000.0f);

	Entity sun = g_CurrentScene->CreateEntity("Sun Test");
	sun.AddComponent<DirectionalLightComponent>();
	sun.AddComponent<SkyLightComponent>().color = Math::ColorF32x3(0.42f, 0.478f, 0.627f);
	sun.GetComponent<TransformComponent>().transform.SetRotation(50.0f, -30.0f, 0.0f);
	
	class TestCameraController : public ScriptableEntity
	{
	public:
		//CTCLASS(TestCameraController); // CT_CLASS(TestCameraController);
		static const char* _GetBehaviorName() { return "TestCameraController"; } // TODO: move to a CTCLASS or CT_CLASS macro or something similar to UCLASS and GDCLASS
		friend class NativeScriptComponent; // optional if you want your function callbacks to be private and still callable

		void OnCreate()
		{
			CT_TRACE("TestCameraController::OnCreate()");
		}
		void OnDestroy()
		{
			CT_TRACE("TestCameraController::OnDestroy()");
		}

		void OnUpdate(float64 deltaTime)
		{
			//CT_TRACE("Updated DT: {}", deltaTime);
			auto& cc = GetComponent<CameraComponent>();
			auto& tc = GetComponent<TransformComponent>();

			if (Input::SimpleInput::GetKey(Input::KeyCode::PageUp)) cc.camera.SetPerspectiveVerticalFOV(cc.camera.GetPerspectiveVerticalFOV() + 0.5f * deltaTime);
			if (Input::SimpleInput::GetKey(Input::KeyCode::PageDown)) cc.camera.SetPerspectiveVerticalFOV(cc.camera.GetPerspectiveVerticalFOV() - 0.5f * deltaTime);

			constexpr const float32 moveSpeed = 1.0f;

			if (Input::SimpleInput::GetKey(Input::KeyCode::Up))
				tc.transform.position.y += moveSpeed * deltaTime;
			if (Input::SimpleInput::GetKey(Input::KeyCode::Down))
				tc.transform.position.y -= moveSpeed * deltaTime;
			if (Input::SimpleInput::GetKey(Input::KeyCode::Right))
				tc.transform.position.x += moveSpeed * deltaTime;
			if (Input::SimpleInput::GetKey(Input::KeyCode::Left))
				tc.transform.position.x -= moveSpeed * deltaTime;
		}
		void OnTick(float64 fixedDeltaTime)
		{
			//CT_TRACE("TICK! {}", fixedDeltaTime);
		}
	};
	class TestPrinter : public ScriptableEntity
	{
	public:
		static const char* _GetBehaviorName() { return "TestPrinter"; }

		void OnCreate()
		{
			CT_VERBOSE("TestPrinter::OnCreate()");
		}
		void OnDestroy()
		{
			CT_VERBOSE("TestPrinter::OnDestroy()");
		}

		void OnUpdate(float64 deltaTime)
		{
			CT_TRACE("*Printer Sounds at an {} interval*", deltaTime);
		}

	public:
		static void BindMethods() {}
	};
	Scripting::NativeScriptDB::RegisterBehavior<TestCameraController>("TestCameraController");
	Scripting::NativeScriptDB::RegisterBehavior<TestPrinter>("TestPrinter");
	camera.AddComponent<NativeScriptComponent>().SetBehaviorWithString("TestCameraController"); //.SetBehavior<TestCameraController>();

	// TODO: implement something like this
	/*
	engine.RegisterFunction("Add", Add); // also engine should probably be general ScriptDB, since we want to be able to bind these functions to scripting languages too
	engine.RegisterFunction("Concatenate", Concatenate);

	int addResult = engine.CallFunction<int>("Add", 3, 5);
	std::string concatResult = engine.CallFunction<std::string>("Concatenate", std::string("Hello, "), std::string("world!"));
	*/

	struct TestClass
	{
		std::string name;
		int64 age;
		bool enabled;

		static TestClass DeserializeJsonOnDemand(simdjson::ondemand::object doc)
		{
			TestClass testClass;

			JSON_READER_ODM_BEGIN()

			JSON_READER_ODM_GET_STRING("name", testClass.name)
			JSON_READER_ODM_GET_INT64("age", testClass.age)
			JSON_READER_ODM_GET_BOOL("enabled", testClass.enabled)

			JSON_READER_ODM_END();

			return testClass;
		}
		static TestClass DeserializeJson(const simdjson::dom::element& doc)
		{
			TestClass testClass;

			//doc["name"].get(testClass.name);
			//doc["age"].get(testClass.age);
			//doc["enabled"].get(testClass.enabled);
			//std::string_view nameView;
			//doc["name"].get(nameView);
			//testClass.name = std::string(nameView);

			JSON_READER_BEGIN()

			JSON_READER_GET_STRING("name", testClass.name)
			JSON_READER_GET_INT64("age", testClass.age)
			JSON_READER_GET_BOOL("enabled", testClass.enabled)

			JSON_READER_END();

			return testClass;
		}

		void SerializeJson(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator) const
		{
			//doc.AddMember("name", rapidjson::Value(this->name.c_str(), allocator), allocator);
			//doc.AddMember("age", this->age, allocator);
			//doc.AddMember("enabled", this->enabled, allocator);

			//rapidjson::Value videoOptions(rapidjson::kObjectType);

			JSON_WRITER_SET_STRING("name", this->name);
			JSON_WRITER_SET_INT64("age", this->age);
			JSON_WRITER_SET_BOOL("enabled", this->enabled);

			//doc.AddMember("VIDEO_OPTIONS", videoOptions, allocator);
		}
	};

	{
		std::string json = R"({"name": "John", "age": 30, "enabled": true})";
		TestClass testJson = JSON::DeserializeObject<TestClass>(json);

		CT_ERROR("Name: {}, Age: {}, Enabled: {};", testJson.name, testJson.age, testJson.enabled);

		std::string jsonString = JSON::SerializeObject<TestClass>(testJson, JSON::SerializerOptions(true));

		CT_WARN("JSON: \n{}", jsonString);
	}
	//{
	//	JSON::BuilderWriter out;
	//	out.Key("CameraComponent").BeginMap();
	//
	//	out.Key("Camera").BeginMap();
	//	out.Key("ProjectionType").Value((int64)1);
	//	out.Key("PerspectiveFOV").Value(60.0f);
	//	out.Key("PerspectiveNear").Value(0.1f);
	//	out.Key("PerspectiveFar").Value(1000.0f);
	//	out.Key("OrthographicSize").Value(10.0f);
	//	out.Key("OrthographicNear").Value(0.01f);
	//	out.Key("OrthographicFar").Value(100.0f);
	//	out.EndMap();  // Camera
	//
	//	out.Key("Primary").Value(true);
	//	out.Key("FixedAspectRatio").Value(false);
	//	out.EndMap();  // CameraComponent
	//
	//	std::string jsonString = out.GetString();
	//
	//	CT_WARN("JSON BUILDER STRING: \n{}", jsonString);
	//}

	// Sol2
	{
		sol::state lua;

		lua.open_libraries(sol::lib::base);

		auto Lua_PrintCpp = [](const std::string& message) {
			CT_INFO("{}", message);
			};

		lua.set_function("PrintCpp", Lua_PrintCpp);

		lua.script(R"(
			PrintCpp('Hello from Lua!')
		)");
	}
	// Angel Script // TODO: doesn't work
	{
		asIScriptEngine* engine = asCreateScriptEngine();
		CT_ASSERT(engine != nullptr, "Failed to create AS Engine!");

		// Register the print function so it can be used by the script
		//auto lambda = [](std::string& msg) { CT_INFO("{}", msg); }; // requires free functions!
		engine->RegisterGlobalFunction("void print(string &in)", asFUNCTION(AS_Print), asCALL_CDECL);

		// Create the script
		const char* script = R"(
			void main()
			{
			    print("Hello from AngelScript!");
			}
		)";

		// Compile the script
		asIScriptModule* mod = engine->GetModule("MyModule", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		mod->Build();

		// Execute the script
		asIScriptContext* ctx = engine->CreateContext();
		ctx->Prepare(mod->GetFunctionByDecl("void main()"));
		ctx->Execute();

		// Clean up
		ctx->Release();
		engine->ShutDownAndRelease();
	}
	// TODO: add squirrel too (it's very similar to Lua in the implementation)
	{
	}

	using namespace Platform;
	MainApplicationSpec = GetApplicationSpecification();
	g_Application = new Application();
	g_Application->gameWindow = &g_Window;
	g_Application->gameLayerStack = &g_LayerStack;
	//GEngine
	//GApp
	//GApplic
	//g_Appli
	//g_App
	//g_Engine
	//GApplication->GameWindow = &GWindow;
	//GApplication->MainApplicationSpec = GetApplicationSpecification();
	// that moment when you want to change your naming convention mid-project...

	//g_Application->MainApplicationSpec >>
	//g_Application->mainApplicationSpec

	g_Window.Create(MainApplicationSpec.windowInfo.width, MainApplicationSpec.windowInfo.height, MainApplicationSpec.windowInfo.defaultTitle.c_str());
	if (MainApplicationSpec.windowInfo.displayMode != DisplayMode::Windowed)
		g_Window.GetBackend()->SetDisplayMode(MainApplicationSpec.windowInfo.displayMode); // TODO: maybe also add SetResolution after this

	// TODO: temporary, should be in render thread.
	Renderer::Initialize(&g_Window);

	IF_EDITOR(g_ImLayer.OnAttach());
	IF_EDITOR(g_ImLayer.Initialize(&g_Window));

	// Push Layers // TODO: make a system that loads a project's layer system features from json file
	IF_EDITOR(g_LayerStack.Push(&g_EditorLayer));
	SimpleInputLayer inputLayer; g_LayerStack.Push(&inputLayer);
	NativeScriptLayer nativeScriptLayer; g_LayerStack.Push(&nativeScriptLayer);
	nativeScriptLayer.StartPlaying(g_CurrentScene);

	::ForgeLoop();

	IF_EDITOR(g_ImLayer.OnDetach());

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

	float64 tickAccumulator = 0.0f; // Tick

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

#ifdef CT_EDITOR_ENABLED
		if (Input::SimpleInput::GetKey(Input::KeyCode::Shift) && Input::SimpleInput::GetKeyDown(Input::KeyCode::F11))
			g_Application->enableEditorUI = !g_Application->enableEditorUI;
#endif

		// Update & Tick (Fixed Update)
		g_LayerStack.Update(g_DeltaTime);

		// while (accumulated time >= fixed time step) probably
		tickAccumulator += g_DeltaTime;
		while (tickAccumulator >= g_Application->fixedTimeStep)
		{
			g_LayerStack.Tick(g_Application->fixedTimeStep);
			tickAccumulator -= g_Application->fixedTimeStep;
		}

		// Render
		Camera* currentCamera = GetCamera();
		Math::Transform* currentCameraTransform = GetCameraTransform();

		Renderer::BeginFrame(g_CurrentScene, currentCamera, currentCameraTransform);
		g_LayerStack.Render();

		Renderer::DrawTest(currentCamera, currentCameraTransform);

		//Renderer::SubmitScene(g_CurrentScene);

		// TODO: implement something like F11: Open/close editor UI and pause the game.
								//Shift + F11: Open / close editor UI without pausing the game.
										//F12: Toggle free - camera mode(detached from the player)
#ifdef CT_EDITOR_ENABLED
		if (g_Application->enableEditorUI) // if GApplication->EditorUIEnabled
		{
			CT_PROFILE_SCOPE("ImGui Render");
			g_ImLayer.Begin();
			g_LayerStack.ImGuiRender();
			g_ImLayer.End();
		}
#endif

		Renderer::EndFrame();

		/*Profiler::ProfileResults::IterateResultsCallback([](const char* key, const float64 time)
		{
			Citrom::Logger::GetLogger()->Log(Citrom::Logger::LogCategory::Core, Citrom::Logger::LogLevel::Trace, "{}(): " "Profiling {} took {} ms!!!!", __func__, key, time * 1000);
		});*/

		//Profiler::ProfileResults::PrintResults();
		//CT_WARN("{}", Profiler::ProfileResults::RetrieveTime("class Citrom::Platform::Window::PollEvents()") * 1000);
	}
}
