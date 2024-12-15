#ifdef CT_PLATFORM_WINDOWS
#include "PlatformWindowBackendWin32.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"
#include "../../../Resources/Resource.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

namespace Citrom::Platform
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
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
			RECT rect;
			GetWindowRect(hWnd, &rect);
			uint32 width, height;
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;

			WindowResizeEvent windowResizeEvent;
			windowResizeEvent.width = width;
			windowResizeEvent.height = height;

			EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowResizeEvent);
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
			if ((lParam & 0x40000000) == 0)
			{
				// check for left and right keys (dirty code)
				// Shift
				if (wParam == VK_SHIFT)
				{
					if (GetKeyState(VK_LSHIFT) & 0x8000) // Check if the left shift key is pressed
					{
						//input_system_register_key_press(CT_KEY_SHIFT);
						//input_system_register_key_press(CT_KEY_LSHIFT);
					}
					else if (GetKeyState(VK_RSHIFT) & 0x8000) // Check if the right shift key is pressed
					{
						//input_system_register_key_press(CT_KEY_SHIFT);
						//input_system_register_key_press(CT_KEY_RSHIFT);
					}
					break;
				}

				// Alt
				if (wParam == VK_MENU)
				{
					if (GetKeyState(VK_LMENU) & 0x8000) // Check if the left menu key is pressed
					{
						//input_system_register_key_press(CT_KEY_ALT);
						//input_system_register_key_press(CT_KEY_LALT);
					}
					else if (GetKeyState(VK_RMENU) & 0x8000) // Check if the right menu key is pressed
					{
						//input_system_register_key_press(CT_KEY_ALT);
						//input_system_register_key_press(CT_KEY_RALT);
					}
					break;
				}

				// Control
				if (wParam == VK_CONTROL)
				{
					if (GetKeyState(VK_LCONTROL) & 0x8000) // Check if the left control key is pressed
					{
						//input_system_register_key_press(CT_KEY_CTRL);
						//input_system_register_key_press(CT_KEY_LCTRL);
					}
					else if (GetKeyState(VK_RCONTROL) & 0x8000) // Check if the right control key is pressed
					{
						//input_system_register_key_press(CT_KEY_CTRL);
						//input_system_register_key_press(CT_KEY_RCTRL);
					}
					break;
				}

				//input_system_register_key_press(win_key_to_input_system(wParam));

				if ((HIWORD(lParam) & KF_REPEAT) != KF_REPEAT) // 0x4000 = 0b0100000000000000 = 16384
				{
					//KeyDownEvent keyDownEvent(wParam);
					//EventBus::GetInstance()->Dispatch<KeyEvents>(keyDownEvent);
				}
			}
			else
			{
				//input_system_register_key_repeat(win_key_to_input_system(wParam));
			}
		}
		break;
		case WM_KEYUP:
		{
			//input_system_register_key_release(win_key_to_input_system(wParam));
		}
		break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

    WindowBackendWin32::WindowBackendWin32()
        : m_WindowShouldClose(false), m_Width(0), m_Height(0), m_ClassName((TCHAR*)TEXT("Class Name")), m_HInstance(GetModuleHandle(NULL)), m_HWnd(0)
    {
    }
    WindowBackendWin32::~WindowBackendWin32()
    {
		DestroyWindow(m_HWnd);
    }

    void WindowBackendWin32::Create(const int width, const int height, const CTL::String& title)
    {
        CT_TRACE("Test: {}, {}, {}", width, height, title.CStr());

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
		CT_CORE_ASSERT(RegisterClassEx(&wc), "Failed to register window class.");

		// TODO: Get Wide String but instead implement a function to convert the string to UTF-16 in the string class!
		int bufferSize = MultiByteToWideChar(CP_UTF8, 0, title.CStr(), -1, NULL, 0);
		CT_CORE_ASSERT(bufferSize != 0, "Failed to calculate buffer size!");

		wchar_t* wideString = new wchar_t[bufferSize];
		CT_CORE_ASSERT(MultiByteToWideChar(CP_UTF8, 0, title.CStr(), -1, wideString, bufferSize), "Failed to convert string!");
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
	void* WindowBackendWin32::Win32TryGetHWnd()
	{
		return static_cast<void*>(m_HWnd);
	}
}
#endif