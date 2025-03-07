#ifdef CT_PLATFORM_WINDOWS
#include "PlatformWindowBackendWin32.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"
#include "../../../Resources/Resource.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#include "Input/KeyboardInput.h"

#include "Renderer/RenderAPI/DirectX11/DX11Device.h" // required for fullscreen
#include "Renderer/RenderAPI/DirectX11/DX11DebugHandler.h"

#include <windowsx.h>

#ifdef CT_EDITOR_ENABLED
#include "Renderer/Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_opengl3.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

namespace Citrom::Platform
{
	//enum class RegisterKeyEventType
	//{
	//	Up,
	//	Down,
	//	Repeat
	//};
	//static void RegisterKeyEvent(RegisterKeyEventType type, WPARAM wParam)
	//{
	//	KeyDownEvent keyDownEvent;
	//	keyDownEvent.keyCode = Input::WinKeyToInputSystem(wParam);
	//
	//	EventBus::GetDispatcher<KeyEvents>()->Dispatch(keyDownEvent);
	//}

	template<typename EventType, typename EventGroup = KeyEvents>
	static void RegisterKeyEvent(Input::KeyCode keyCode)
	{
		EventType keyEvent;
		keyEvent.keyCode = keyCode;

		EventBus::GetDispatcher<EventGroup>()->Dispatch(keyEvent);
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		#ifdef CT_EDITOR_ENABLED
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		#endif
		switch (msg)
		{
		// Window Events Begin
		case WM_CLOSE:
		{
			WindowCloseEvent windowCloseEvent;
			windowCloseEvent.exitCode = 69;

			EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowCloseEvent);

			PostQuitMessage(69);
		}
		break;
		case WM_SIZE:
		{
			//RECT rect;
			//GetWindowRect(hWnd, &rect);
			//uint32 width, height;
			//width = rect.right - rect.left;
			//height = rect.bottom - rect.top;

			const uint32 width = (UINT)LOWORD(lParam);
			const uint32 height = (UINT)HIWORD(lParam);

			WindowResizeEvent windowResizeEvent;
			windowResizeEvent.width = width;
			windowResizeEvent.height = height;

			EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowResizeEvent);

			// TODO: figure out whether do i really need to do this, because
			// ideally it should not need to be called
			#ifdef CT_EDITOR_ENABLED
			if (ImGui::GetCurrentContext() != NULL)
				ImGui::GetIO().DisplaySize = ImVec2(width, height);
			#endif
		}
		break;
		case WM_MOVE:
		{
			auto xPos = (int)(short)LOWORD(lParam);   // horizontal position 
			auto yPos = (int)(short)HIWORD(lParam);   // vertical position 

			WindowMoveEvent windowMoveEvent;
			windowMoveEvent.x = xPos;
			windowMoveEvent.y = yPos;

			EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowMoveEvent);
		}
		break;
		case WM_SETFOCUS:
		{
			WindowFocusEvent windowFocusEvent;
			windowFocusEvent.state = true;

			EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowFocusEvent);
		}
		break;
		case WM_KILLFOCUS:
		{
			WindowFocusEvent windowFocusEvent;
			windowFocusEvent.state = false;

			EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowFocusEvent);
		}
		break;
		// Window Events End
		// Mouse Events Begin
		// Left button
		case WM_LBUTTONDOWN:
		{
			//POINTS pt = MAKEPOINTS(lParam);

			MouseDownEvent mouseDownEvent;
			mouseDownEvent.mouseButton = Input::MouseButton::LeftButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseDownEvent);
		}
		break;
		case WM_LBUTTONUP:
		{
			//POINTS pt = MAKEPOINTS(lParam);

			MouseUpEvent mouseUpEvent;
			mouseUpEvent.mouseButton = Input::MouseButton::LeftButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseUpEvent);
		}
		break;
		// Right button
		case WM_RBUTTONDOWN:
		{
			//POINTS pt = MAKEPOINTS(lParam);

			MouseDownEvent mouseDownEvent;
			mouseDownEvent.mouseButton = Input::MouseButton::RightButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseDownEvent);
		}
		break;
		case WM_RBUTTONUP:
		{
			//POINTS pt = MAKEPOINTS(lParam);

			MouseUpEvent mouseUpEvent;
			mouseUpEvent.mouseButton = Input::MouseButton::RightButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseUpEvent);
		}
		break;
		// Middle button
		case WM_MBUTTONDOWN:
		{
			//POINTS pt = MAKEPOINTS(lParam);

			MouseDownEvent mouseDownEvent;
			mouseDownEvent.mouseButton = Input::MouseButton::MiddleButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseDownEvent);
		}
		break;
		case WM_MBUTTONUP:
		{
			//POINTS pt = MAKEPOINTS(lParam);

			MouseUpEvent mouseUpEvent;
			mouseUpEvent.mouseButton = Input::MouseButton::MiddleButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseUpEvent);
		}
		break;
		/* case WM_LBUTTONDBLCLK:	// add CS_DBLCLKS
				CT_LOGGER_LOG(Input::MouseButton::LeftButton);
				break;*/
		case WM_XBUTTONDOWN:
		{
			int button = GET_XBUTTON_WPARAM(wParam);  // Get the button (XBUTTON1 or XBUTTON2 etc.)
			EventMouseButton eventButton;
			switch (button) 
			{
				case XBUTTON1:
					eventButton = Input::MouseButton::Mouse4;
					break;
				case XBUTTON2:
					eventButton = Input::MouseButton::Mouse5;
					break;

				// TODO: untested
				default:
				{
					// Handle extra buttons (6 to 20)
					int extraButtonIndex = button - XBUTTON2;
					if (extraButtonIndex >= 1 && extraButtonIndex <= 12) // buttons 6-20
					{
						eventButton = static_cast<Input::MouseButton>(static_cast<int>(Input::MouseButton::Mouse6) + (extraButtonIndex - 1));
					}
					else
					{
						eventButton = Input::MouseButton::Null;
					}
				}
				break;
			}
			MouseDownEvent mouseDownEvent;
			mouseDownEvent.mouseButton = eventButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseDownEvent);
		}
		break;
		case WM_XBUTTONUP:
		{
			int button = GET_XBUTTON_WPARAM(wParam);  // Get the button (XBUTTON1 or XBUTTON2 etc.)
			EventMouseButton eventButton;
			switch (button)
			{
			case XBUTTON1:
				eventButton = Input::MouseButton::Mouse4;
				break;
			case XBUTTON2:
				eventButton = Input::MouseButton::Mouse5;
				break;

				// TODO: untested
			default:
			{
				// Handle extra buttons (6 to 20)
				int extraButtonIndex = button - XBUTTON2;
				if (extraButtonIndex >= 1 && extraButtonIndex <= 12) // buttons 6-20
				{
					eventButton = static_cast<Input::MouseButton>(static_cast<int>(Input::MouseButton::Mouse6) + (extraButtonIndex - 1));
				}
				else
				{
					eventButton = Input::MouseButton::Null;
				}
			}
			break;
			}
			MouseUpEvent mouseUpEvent;
			mouseUpEvent.mouseButton = eventButton;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseUpEvent);
		}
		break;
		case WM_MOUSEMOVE:
		{
			auto xPos = GET_X_LPARAM(lParam); // prefer instead of LOWORD because systems with multiple monitors might have negative coords
			auto yPos = GET_Y_LPARAM(lParam);

			MouseMoveEvent mouseMoveEvent;
			mouseMoveEvent.x = xPos;
			mouseMoveEvent.y = yPos;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseMoveEvent);
		}
		break;
		case WM_MOUSEWHEEL:
		{
			//auto xPos = GET_X_LPARAM(lParam); // prefer instead of LOWORD because systems with multiple monitors might have negative coords
			//auto yPos = GET_Y_LPARAM(lParam);

			//auto fwKeys = GET_KEYSTATE_WPARAM(wParam);
			auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam); // V/Y Delta

			MouseScrollEvent mouseScrollEvent;
			mouseScrollEvent.x = 0;
			mouseScrollEvent.y = (float32)zDelta / 120.0f;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseScrollEvent);
		}
		break;
		case WM_MOUSEHWHEEL:
		{
			auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam); // H/X Delta

			MouseScrollEvent mouseScrollEvent;
			mouseScrollEvent.x = (float32)zDelta / 120.0f;
			mouseScrollEvent.y = 0;

			EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseScrollEvent);
		}
		break;
		// Mouse Events End
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) != WA_INACTIVE)
			{
				// Bring the window to the top
				SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
		}
		break;
		case WM_SYSKEYDOWN:
		{
			// Alt + Enter
			if (wParam == VK_RETURN && (lParam & (1 << 29)))
			{
				//toggle_fullscreen();
			}
		}
		break;
		case WM_KEYDOWN:
		{
			// check for key repeats
			if ((lParam & KF_REPEAT) == 0)
			{
				static const CTL::HashMap<WPARAM, std::pair<int, int>> modifierKeys = 
				{
						{ VK_SHIFT,   {VK_LSHIFT,	VK_RSHIFT}		},
						{ VK_MENU,    {VK_LMENU,	VK_RMENU}		},
						{ VK_CONTROL, {VK_LCONTROL, VK_RCONTROL}	}
				};

				for (const auto& modifierKeyPair : modifierKeys)
				{
					//if (wParam == VK_MENU)
					if (GetKeyState(modifierKeyPair.second.first) & 0x8000) // Left Key
					{
						RegisterKeyEvent<KeyDownEvent>(Input::WinKeyToInputSystem(modifierKeyPair.second.first));
					}
					else if (GetKeyState(modifierKeyPair.second.second) & 0x8000) // Check if right key is pressed
					{
						RegisterKeyEvent<KeyDownEvent>(Input::WinKeyToInputSystem(modifierKeyPair.second.second));
					}
				}

				//input_system_register_key_press(win_key_to_input_system(wParam));

				if ((HIWORD(lParam) & KF_REPEAT) != KF_REPEAT) // 0x40000000 = 0b0100000000000000 = 16384
				{
					RegisterKeyEvent<KeyDownEvent>(Input::WinKeyToInputSystem(wParam));
				}
			}
			else
			{
				RegisterKeyEvent<KeyRepeatEvent>(Input::WinKeyToInputSystem(wParam));
			}
		}
		break;
		case WM_KEYUP:
		{
			RegisterKeyEvent<KeyUpEvent>(Input::WinKeyToInputSystem(wParam));

			CT_CORE_VERBOSE("KeyUp WPARAM: {}", wParam);
			CT_CORE_VERBOSE("KeyUp LPARAM: {}", lParam);
		}
		break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	/* // TODO: implement multiple fullscreen options
	void platform_update_screen()
	{
		if (platformPtr->surfaceData.fullscreen)
		{
			// Retrieve the primary display device name
			DISPLAY_DEVICE dd = { 0 };
			dd.cb = sizeof(DISPLAY_DEVICE);
			EnumDisplayDevices(NULL, 0, &dd, 0);
	
			// Retrieve current display settings
			DEVMODE dm = { 0 };
			dm.dmSize = sizeof(DEVMODE);
			EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);
	
			// Get monitor width and height
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
			CT_CORE_ASSERT(ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL, "Failed to change display mode.");
	
			SetWindowLongPtr(platformPtr->internalData->hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(platformPtr->internalData->hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);
		}
		else
		{
			// Restore window style to overlapped window
			SetWindowLongPtr(platformPtr->internalData->hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	
			// Adjust window size and position as desired
			SetWindowPos(platformPtr->internalData->hWnd, HWND_TOP, platformPtr->surfaceData.x, platformPtr->surfaceData.y, platformPtr->surfaceData.width, platformPtr->surfaceData.height, SWP_FRAMECHANGED);
		}
	}
	*/

    WindowBackendWin32::WindowBackendWin32()
        : m_WindowShouldClose(false), m_Width(0), m_Height(0), m_ClassName((TCHAR*)TEXT("Citrom Tech Main Window")), m_HInstance(GetModuleHandle(NULL)), m_HWnd(0)
    {
    }
    WindowBackendWin32::~WindowBackendWin32()
    {
		DestroyWindow(m_HWnd);
    }

    void WindowBackendWin32::Create(const int width, const int height, const CTL::String& title)
    {
        CT_TRACE("Test: {}, {}, {}", width, height, title.CStr());
		//WindowResizeEvent initialResize;
		//initialResize.width = width;
		//initialResize.height = height;
		//EventBus::GetDispatcher<WindowEvents>()->Dispatch(initialResize);

		//m_ClassName = (TCHAR*)TEXT("Class Name");
		//m_HInstance = GetModuleHandle(NULL); // supposedly shouldn't be ran in DLL

		// register windows class
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_HInstance;
		wc.hIcon = LoadIcon(m_HInstance, MAKEINTRESOURCE(IDI_CITROMFORGEICON)); //LoadIcon(m_HInstance, IDI_APPLICATION); // LoadIcon(m_HInstance, MAKEINTRESOURCE(IDI_CITROMFORGEICON));
		wc.hbrBackground = nullptr; // LoadCursor(NULL, IDC_ARROW);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = m_ClassName; //L"title";
		wc.hIconSm = nullptr;
		CT_CORE_VERIFY(RegisterClassEx(&wc), "Failed to register window class.");

		// TODO: Get Wide String but instead implement a function to convert the string to UTF-16 in the string class!
		int bufferSize = MultiByteToWideChar(CP_UTF8, 0, title.CStr(), -1, NULL, 0);
		CT_CORE_ASSERT(bufferSize != 0, "Failed to calculate buffer size!");

		wchar_t* wideString = new wchar_t[bufferSize];
		CT_CORE_VERIFY(MultiByteToWideChar(CP_UTF8, 0, title.CStr(), -1, wideString, bufferSize), "Failed to convert string!");
		// ------------------------------------------------------------------------------------------------------------

		// create window instance
		m_HWnd = CreateWindowEx(0, m_ClassName, wideString, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, m_HInstance, nullptr);

		CT_CORE_ASSERT(m_HWnd, "Failed to create window instance.");

		delete[] wideString;

		// platform_get_time() initialization
		//LARGE_INTEGER frequency;
		//QueryPerformanceFrequency(&frequency);
		//sg_ClockFrequency = 1.0 / (float64)frequency.QuadPart;
		//QueryPerformanceCounter(&sg_StartTime);
		// -------------------------------
    }
    bool WindowBackendWin32::WindowShouldClose() const
    {
        return m_WindowShouldClose;
    }
    void WindowBackendWin32::PollEvents()
    {
		// Update Window
		ShowWindow(m_HWnd, SW_SHOW);
		UpdateWindow(m_HWnd);

		// Poll Events
		MSG msg;
		BOOL gResult;
		while (gResult = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Quit due to an error
			if (gResult == -1)
			{
				CT_CORE_FATAL("PeekMessage encountered an error.");
				m_WindowShouldClose = true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Quit gracefully
			if (msg.message == WM_QUIT)
			{
				m_WindowShouldClose = true;
				break;
			}
		}
    }

	void WindowBackendWin32::SetDisplayMode(DisplayMode displayMode, const uint32 refreshRate)
	{
#ifdef CT_PLATFORM_WINDOWS
#define SET_DIRECTX_FULLSCREEN(STATE) if (RenderAPI::GraphicsAPIManager::GetGraphicsAPI() == RenderAPI::GraphicsAPI::DirectX11 && D3D11SWAPCHAIN) {HRESULT hr; DXCallHR(D3D11SWAPCHAIN->SetFullscreenState((STATE), NULL));} \
								 //else if (RenderAPI::GraphicsAPIManager::GetGraphicsAPI() == RenderAPI::GraphicsAPI::DirectX12 && D3D12SWAPCHAIN) {HRESULT hr; DXCallHR(D3D12SWAPCHAIN->SetFullscreenState((STATE), NULL));}
#else
#define SET_DIRECTX_FULLSCREEN(...)
#endif

		m_DisplayMode = displayMode;
		switch (displayMode)
		{
			default:
			case DisplayMode::Windowed:
			{
				SET_DIRECTX_FULLSCREEN(FALSE);

				// Restore window style to overlapped window
				SetWindowLongPtr(m_HWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

				SetWindowPos(m_HWnd, HWND_TOP, 0, 0, PLATFORM_DEFAULT_WIDTH, PLATFORM_DEFAULT_HEIGHT, SWP_FRAMECHANGED);
			}
			break;
			case DisplayMode::Borderless:
			{
				SET_DIRECTX_FULLSCREEN(FALSE);

				// Retrieve the primary display device name
				/*DISPLAY_DEVICE dd = {0};
				dd.cb = sizeof(DISPLAY_DEVICE);
				EnumDisplayDevices(NULL, 0, &dd, 0);

				// Retrieve current display settings
				DEVMODE dm = { 0 };
				dm.dmSize = sizeof(DEVMODE);
				EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);*/

				// Get monitor width and height
				int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				int screenHeight = GetSystemMetrics(SM_CYSCREEN);

				//CT_CORE_VERIFY(ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL, "Failed to change display mode.");

				SetWindowLongPtr(m_HWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
				SetWindowPos(m_HWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);

				SetForegroundWindow(m_HWnd); // Brings the window to the front
				SetFocus(m_HWnd);
			}
			break;
			case DisplayMode::Fullscreen:
			{
				//SET_DIRECTX_FULLSCREEN(TRUE);

				// Retrieve the primary display device name
				DISPLAY_DEVICE dd = { 0 };
				dd.cb = sizeof(DISPLAY_DEVICE);
				EnumDisplayDevices(NULL, 0, &dd, 0);

				// Retrieve current display settings
				DEVMODE dm = { 0 };
				dm.dmSize = sizeof(DEVMODE);
				CT_CORE_VERIFY(EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm), "Failed to Enumerate Display Settings.");

				DWORD maxFrequency = 0;
				if (refreshRate == 0)
				{
					// Enumerate supported display modes
					DEVMODE dmEnum = { 0 };
					dmEnum.dmSize = sizeof(DEVMODE);

					for (int iModeNum = 0; EnumDisplaySettings(dd.DeviceName, iModeNum, &dmEnum); iModeNum++)
					{
						if (dmEnum.dmPelsWidth == dm.dmPelsWidth &&
							dmEnum.dmPelsHeight == dm.dmPelsHeight)
						{
							if (dmEnum.dmDisplayFrequency > maxFrequency)
							{
								maxFrequency = dmEnum.dmDisplayFrequency;
							}
						}
					}
				}
				else
				{
					maxFrequency = refreshRate;
				}

				// Fullscreen
				dm.dmPelsWidth = GetSystemMetrics(SM_CXSCREEN);
				dm.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
				dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
				dm.dmDisplayFrequency = maxFrequency;

				CT_CORE_VERIFY(ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL, "Failed to change display mode.");
				//CT_CORE_VERIFY(ChangeDisplaySettingsEx(dd.DeviceName, &dm, nullptr, CDS_FULLSCREEN, nullptr) == DISP_CHANGE_SUCCESSFUL, "Failed to change display mode.");

				// Remove window borders and set fullscreen size
				//SetWindowLongPtr(m_HWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
				//SetWindowPos(m_HWnd, HWND_TOP, 0, 0, dm.dmPelsWidth, dm.dmPelsHeight, SWP_FRAMECHANGED);

				SET_DIRECTX_FULLSCREEN(TRUE);
			}
			break;
		}
	}
	void WindowBackendWin32::SetResolution(const uint32 width, const uint32 height, const uint32 refreshRate, const int xPos, const int yPos)
	{
		DEVMODE dm = {};
		dm.dmSize = sizeof(dm);

		DISPLAY_DEVICE dd = { 0 };
		dd.cb = sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(NULL, 0, &dd, 0);

		DWORD maxFrequency = 0;
		if (refreshRate == 0)
		{
			// Enumerate supported display modes
			DEVMODE dmEnum = { 0 };
			dmEnum.dmSize = sizeof(DEVMODE);

			for (int iModeNum = 0; EnumDisplaySettings(dd.DeviceName, iModeNum, &dmEnum); iModeNum++)
			{
				if (dmEnum.dmPelsWidth == dm.dmPelsWidth &&
					dmEnum.dmPelsHeight == dm.dmPelsHeight)
				{
					if (dmEnum.dmDisplayFrequency > maxFrequency)
					{
						maxFrequency = dmEnum.dmDisplayFrequency;
					}
				}
			}
		}
		else
		{
			maxFrequency = refreshRate;
		}

		dm.dmPelsWidth = width;
		dm.dmPelsHeight = height;
		//devMode.dmBitsPerPel = 32;
		dm.dmDisplayFrequency = maxFrequency;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

		HMONITOR hMonitor = MonitorFromWindow(m_HWnd, MONITOR_DEFAULTTOPRIMARY);
		if (hMonitor != nullptr)
		{
			// Fullscreen mode
			ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
			SetWindowPos(m_HWnd, HWND_TOP, xPos, yPos, width, height, SWP_NOZORDER | SWP_FRAMECHANGED);
		}
		else
		{
			// Windowed mode
			SetWindowPos(m_HWnd, HWND_TOP, xPos, yPos, width, height, SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}
	Resolution WindowBackendWin32::GetResolution()
	{
		if (m_WindowShouldClose)
			return Resolution();

		Resolution resolution = {};

		RECT rect;
		CT_CORE_VERIFY(GetWindowRect(m_HWnd, &rect), "Failed to get Window Rect!"); // GetWidth(), GetHeight() return the drawable area..

		resolution.width = rect.right - rect.left;
		resolution.height = rect.bottom - rect.top;

		// Refresh Rate
		HMONITOR hMonitor = MonitorFromWindow(m_HWnd, MONITOR_DEFAULTTOPRIMARY);
		if (hMonitor != nullptr)
		{
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(monitorInfo);
			GetMonitorInfo(hMonitor, &monitorInfo);

			DEVMODE devMode = {};
			devMode.dmSize = sizeof(devMode);
			EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
			resolution.refreshRate = devMode.dmDisplayFrequency;
		}
		else
		{
			resolution.refreshRate = 0;
		}

		return resolution;
	}

	int WindowBackendWin32::GetWidth()
	{
		RECT rect;
		CT_CORE_VERIFY(GetClientRect(m_HWnd, &rect), "Failed to get Client Rect!"); // GetWindowRect()
		m_Width = rect.right - rect.left;
		//m_Height = rect.bottom - rect.top;
		return m_Width;
	}
	int WindowBackendWin32::GetHeight()
	{
		RECT rect;
		CT_CORE_VERIFY(GetClientRect(m_HWnd, &rect), "Failed to get Client Rect!"); // GetWindowRect()
		//m_Width = rect.right - rect.left;
		m_Height = rect.bottom - rect.top;
		return m_Height;
	}

	void* WindowBackendWin32::Win32TryGetHWnd()
	{
		return static_cast<void*>(m_HWnd);
	}

	// ImGui
#ifdef CT_EDITOR_ENABLED
	void WindowBackendWin32::ImGuiInitialize()
	{
		RenderAPI::GraphicsAPI graphicsAPI = RenderAPI::GraphicsAPIManager::GetGraphicsAPI();
		if (graphicsAPI == RenderAPI::GraphicsAPI::DirectX11)
		{
			CT_CORE_VERIFY(ImGui_ImplWin32_Init(this->Win32TryGetHWnd()), "Failed to initialize ImGui Win32 implementation.");
		}
		else if (graphicsAPI == RenderAPI::GraphicsAPI::OpenGL)
		{
			// if win32 & opengl
			CT_CORE_VERIFY(ImGui_ImplWin32_InitForOpenGL(this->Win32TryGetHWnd()), "Failed to Initialize ImGui Win32 implementation for OpenGL.");
			//CT_CORE_VERIFY(ImGui_ImplOpenGL3_Init("#version 460"), "Failed to Initialize ImGui OpenGL 4.6 implementation.");
		}
	}
	void WindowBackendWin32::ImGuiTerminate()
	{
		RenderAPI::GraphicsAPI graphicsAPI = RenderAPI::GraphicsAPIManager::GetGraphicsAPI();
		if (graphicsAPI == RenderAPI::GraphicsAPI::DirectX11)
		{
			ImGui_ImplDX11_Shutdown();
		}
		else if (graphicsAPI == RenderAPI::GraphicsAPI::OpenGL)
		{
			ImGui_ImplOpenGL3_Shutdown();
		}

		ImGui_ImplWin32_Shutdown();
	}
#else
	void WindowBackendWin32::ImGuiInitialize() {}
	void WindowBackendWin32::ImGuiTerminate() {}
#endif
}
#endif