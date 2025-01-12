#include "KeyboardInput.h"

#include "GLFW/glfw3.h"

#ifdef CT_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsIncludes.h"
#endif

namespace Citrom::Input
{
#define CASE(x, y) case x:\
					   return y;\
					   break;

	KeyCode GLFWKeyToInputSystem(int key)
	{
		switch (key)
		{
			default: return KeyCode::Null; break;

			CASE(GLFW_KEY_SPACE, KeyCode::Space);
			CASE(GLFW_KEY_APOSTROPHE, KeyCode::Apostrophe);   // '
			CASE(GLFW_KEY_COMMA, KeyCode::Comma);            // ,
			CASE(GLFW_KEY_MINUS, KeyCode::AlphaMinus);       // -
			CASE(GLFW_KEY_PERIOD, KeyCode::Period);          // .
			CASE(GLFW_KEY_SLASH, KeyCode::Slash);            // /
			CASE(GLFW_KEY_0, KeyCode::Alpha0);
			CASE(GLFW_KEY_1, KeyCode::Alpha1);
			CASE(GLFW_KEY_2, KeyCode::Alpha2);
			CASE(GLFW_KEY_3, KeyCode::Alpha3);
			CASE(GLFW_KEY_4, KeyCode::Alpha4);
			CASE(GLFW_KEY_5, KeyCode::Alpha5);
			CASE(GLFW_KEY_6, KeyCode::Alpha6);
			CASE(GLFW_KEY_7, KeyCode::Alpha7);
			CASE(GLFW_KEY_8, KeyCode::Alpha8);
			CASE(GLFW_KEY_9, KeyCode::Alpha9);
			CASE(GLFW_KEY_SEMICOLON, KeyCode::SemiColon);    // ;
			CASE(GLFW_KEY_EQUAL, KeyCode::AlphaEqual);       // =
			CASE(GLFW_KEY_A, KeyCode::A);
			CASE(GLFW_KEY_B, KeyCode::B);
			CASE(GLFW_KEY_C, KeyCode::C);
			CASE(GLFW_KEY_D, KeyCode::D);
			CASE(GLFW_KEY_E, KeyCode::E);
			CASE(GLFW_KEY_F, KeyCode::F);
			CASE(GLFW_KEY_G, KeyCode::G);
			CASE(GLFW_KEY_H, KeyCode::H);
			CASE(GLFW_KEY_I, KeyCode::I);
			CASE(GLFW_KEY_J, KeyCode::J);
			CASE(GLFW_KEY_K, KeyCode::K);
			CASE(GLFW_KEY_L, KeyCode::L);
			CASE(GLFW_KEY_M, KeyCode::M);
			CASE(GLFW_KEY_N, KeyCode::N);
			CASE(GLFW_KEY_O, KeyCode::O);
			CASE(GLFW_KEY_P, KeyCode::P);
			CASE(GLFW_KEY_Q, KeyCode::Q);
			CASE(GLFW_KEY_R, KeyCode::R);
			CASE(GLFW_KEY_S, KeyCode::S);
			CASE(GLFW_KEY_T, KeyCode::T);
			CASE(GLFW_KEY_U, KeyCode::U);
			CASE(GLFW_KEY_V, KeyCode::V);
			CASE(GLFW_KEY_W, KeyCode::W);
			CASE(GLFW_KEY_X, KeyCode::X);
			CASE(GLFW_KEY_Y, KeyCode::Y);
			CASE(GLFW_KEY_Z, KeyCode::Z);
			CASE(GLFW_KEY_LEFT_BRACKET, KeyCode::LeftBracket); // [
			CASE(GLFW_KEY_BACKSLASH, KeyCode::BackSlash);
			CASE(GLFW_KEY_RIGHT_BRACKET, KeyCode::RightBracket); // ]
			CASE(GLFW_KEY_GRAVE_ACCENT, KeyCode::BackQuote);    // `
			//CASE(GLFW_KEY_WORLD_1, KeyCode::World1);           // non-US #1
			//CASE(GLFW_KEY_WORLD_2, KeyCode::World2);           // non-US #2

			// Function keys
			CASE(GLFW_KEY_ESCAPE, KeyCode::Escape);
			CASE(GLFW_KEY_ENTER, KeyCode::Return);
			CASE(GLFW_KEY_TAB, KeyCode::Tab);
			CASE(GLFW_KEY_BACKSPACE, KeyCode::Backspace);
			CASE(GLFW_KEY_INSERT, KeyCode::Insert);
			CASE(GLFW_KEY_DELETE, KeyCode::Delete);
			CASE(GLFW_KEY_RIGHT, KeyCode::Right);
			CASE(GLFW_KEY_LEFT, KeyCode::Left);
			CASE(GLFW_KEY_DOWN, KeyCode::Down);
			CASE(GLFW_KEY_UP, KeyCode::Up);
			CASE(GLFW_KEY_PAGE_UP, KeyCode::PageUp);
			CASE(GLFW_KEY_PAGE_DOWN, KeyCode::PageDown);
			CASE(GLFW_KEY_HOME, KeyCode::Home);
			CASE(GLFW_KEY_END, KeyCode::End);
			CASE(GLFW_KEY_CAPS_LOCK, KeyCode::CapsLock);
			CASE(GLFW_KEY_SCROLL_LOCK, KeyCode::ScrollLock);
			CASE(GLFW_KEY_NUM_LOCK, KeyCode::NumLock);
			CASE(GLFW_KEY_PRINT_SCREEN, KeyCode::PrintScreen);
			CASE(GLFW_KEY_PAUSE, KeyCode::Pause);
			CASE(GLFW_KEY_F1, KeyCode::F1);
			CASE(GLFW_KEY_F2, KeyCode::F2);
			CASE(GLFW_KEY_F3, KeyCode::F3);
			CASE(GLFW_KEY_F4, KeyCode::F4);
			CASE(GLFW_KEY_F5, KeyCode::F5);
			CASE(GLFW_KEY_F6, KeyCode::F6);
			CASE(GLFW_KEY_F7, KeyCode::F7);
			CASE(GLFW_KEY_F8, KeyCode::F8);
			CASE(GLFW_KEY_F9, KeyCode::F9);
			CASE(GLFW_KEY_F10, KeyCode::F10);
			CASE(GLFW_KEY_F11, KeyCode::F11);
			CASE(GLFW_KEY_F12, KeyCode::F12);
			CASE(GLFW_KEY_F13, KeyCode::F13);
			CASE(GLFW_KEY_F14, KeyCode::F14);
			CASE(GLFW_KEY_F15, KeyCode::F15);
			CASE(GLFW_KEY_F16, KeyCode::F16);
			CASE(GLFW_KEY_F17, KeyCode::F17);
			CASE(GLFW_KEY_F18, KeyCode::F18);
			CASE(GLFW_KEY_F19, KeyCode::F19);
			CASE(GLFW_KEY_F20, KeyCode::F20);
			CASE(GLFW_KEY_F21, KeyCode::F21);
			CASE(GLFW_KEY_F22, KeyCode::F22);
			CASE(GLFW_KEY_F23, KeyCode::F23);
			CASE(GLFW_KEY_F24, KeyCode::F24);
			//CASE(GLFW_KEY_F25, KeyCode::F25);

			// Keypad
			CASE(GLFW_KEY_KP_0, KeyCode::NumPad0);
			CASE(GLFW_KEY_KP_1, KeyCode::NumPad1);
			CASE(GLFW_KEY_KP_2, KeyCode::NumPad2);
			CASE(GLFW_KEY_KP_3, KeyCode::NumPad3);
			CASE(GLFW_KEY_KP_4, KeyCode::NumPad4);
			CASE(GLFW_KEY_KP_5, KeyCode::NumPad5);
			CASE(GLFW_KEY_KP_6, KeyCode::NumPad6);
			CASE(GLFW_KEY_KP_7, KeyCode::NumPad7);
			CASE(GLFW_KEY_KP_8, KeyCode::NumPad8);
			CASE(GLFW_KEY_KP_9, KeyCode::NumPad9);
			CASE(GLFW_KEY_KP_DECIMAL, KeyCode::Decimal);
			CASE(GLFW_KEY_KP_DIVIDE, KeyCode::Divide);
			CASE(GLFW_KEY_KP_MULTIPLY, KeyCode::Multiply);
			CASE(GLFW_KEY_KP_SUBTRACT, KeyCode::Subtract);
			CASE(GLFW_KEY_KP_ADD, KeyCode::Add);
			//CASE(GLFW_KEY_KP_ENTER, KeyCode::NumPadEnter);
			//CASE(GLFW_KEY_KP_EQUAL, KeyCode::NumPadEqual);

			// Modifier keys
			CASE(GLFW_KEY_LEFT_SHIFT, KeyCode::LShift);
			CASE(GLFW_KEY_LEFT_CONTROL, KeyCode::LCtrl);
			CASE(GLFW_KEY_LEFT_ALT, KeyCode::LAlt);
			CASE(GLFW_KEY_LEFT_SUPER, KeyCode::Start);
			CASE(GLFW_KEY_RIGHT_SHIFT, KeyCode::RShift);
			CASE(GLFW_KEY_RIGHT_CONTROL, KeyCode::RCtrl);
			CASE(GLFW_KEY_RIGHT_ALT, KeyCode::RAlt);
			CASE(GLFW_KEY_RIGHT_SUPER, KeyCode::Start);
			CASE(GLFW_KEY_MENU, KeyCode::Alt);
		}
		return KeyCode::Null;
	}

#ifdef CT_PLATFORM_WINDOWS
    KeyCode WinKeyToInputSystem(size_t key)
    {
        WPARAM winKey = static_cast<WPARAM>(key);

		switch (winKey)
		{
			default: return KeyCode::Null; break;

			CASE('A', KeyCode::A);
			CASE('B', KeyCode::B);
			CASE('C', KeyCode::C);
			CASE('D', KeyCode::D);
			CASE('E', KeyCode::E);
			CASE('F', KeyCode::F);
			CASE('G', KeyCode::G);
			CASE('H', KeyCode::H);
			CASE('I', KeyCode::I);
			CASE('J', KeyCode::J);
			CASE('K', KeyCode::K);
			CASE('L', KeyCode::L);
			CASE('M', KeyCode::M);
			CASE('N', KeyCode::N);
			CASE('O', KeyCode::O);
			CASE('P', KeyCode::P);
			CASE('Q', KeyCode::Q);
			CASE('R', KeyCode::R);
			CASE('S', KeyCode::S);
			CASE('T', KeyCode::T);
			CASE('U', KeyCode::U);
			CASE('V', KeyCode::V);
			CASE('W', KeyCode::W);
			CASE('X', KeyCode::X);
			CASE('Y', KeyCode::Y);
			CASE('Z', KeyCode::Z);

			CASE('0', KeyCode::Alpha0);
			CASE('1', KeyCode::Alpha1);
			CASE('2', KeyCode::Alpha2);
			CASE('3', KeyCode::Alpha3);
			CASE('4', KeyCode::Alpha4);
			CASE('5', KeyCode::Alpha5);
			CASE('6', KeyCode::Alpha6);
			CASE('7', KeyCode::Alpha7);
			CASE('8', KeyCode::Alpha8);
			CASE('9', KeyCode::Alpha9);

			CASE(192, KeyCode::BackQuote);
			CASE(189, KeyCode::AlphaMinus);
			CASE(187, KeyCode::AlphaEqual);
			CASE(220, KeyCode::BackSlash);
			CASE(191, KeyCode::Slash);
			CASE(219, KeyCode::LeftBracket);
			CASE(221, KeyCode::RightBracket);
			CASE(186, KeyCode::SemiColon);
			CASE(222, KeyCode::Apostrophe);
			CASE(188, KeyCode::Comma);
			CASE(190, KeyCode::Period);

			CASE(VK_BACK, KeyCode::Backspace);
			CASE(VK_TAB, KeyCode::Tab);
			CASE(VK_RETURN, KeyCode::Return);

			CASE(VK_LSHIFT, KeyCode::LShift);
			CASE(VK_RSHIFT, KeyCode::RShift);
			CASE(VK_SHIFT, KeyCode::Shift);

			CASE(VK_LCONTROL, KeyCode::LCtrl);
			CASE(VK_RCONTROL, KeyCode::RCtrl);
			CASE(VK_CONTROL, KeyCode::Ctrl);

			CASE(VK_LMENU, KeyCode::LAlt);
			CASE(VK_RMENU, KeyCode::RAlt);
			CASE(VK_MENU, KeyCode::Alt);

			CASE(VK_PAUSE, KeyCode::Pause);
			CASE(VK_CAPITAL, KeyCode::CapsLock);
			CASE(VK_ESCAPE, KeyCode::Escape);
			CASE(VK_SPACE, KeyCode::Space);
			CASE(VK_PRIOR, KeyCode::PageUp);
			CASE(VK_NEXT, KeyCode::PageDown);
			CASE(VK_END, KeyCode::End);
			CASE(VK_HOME, KeyCode::Home);
			CASE(VK_LEFT, KeyCode::Left);
			CASE(VK_UP, KeyCode::Up);
			CASE(VK_RIGHT, KeyCode::Right);
			CASE(VK_DOWN, KeyCode::Down);
			CASE(VK_SNAPSHOT, KeyCode::PrintScreen);
			CASE(VK_INSERT, KeyCode::Insert);
			CASE(VK_DELETE, KeyCode::Delete);
			CASE(VK_LWIN, KeyCode::Start);		// TODO: decide if you want this
			CASE(VK_RWIN, KeyCode::Start);
			CASE(VK_APPS, KeyCode::Apps);
			CASE(VK_SLEEP, KeyCode::Null);
			CASE(VK_NUMPAD0, KeyCode::NumPad0);
			CASE(VK_NUMPAD1, KeyCode::NumPad1);
			CASE(VK_NUMPAD2, KeyCode::NumPad2);
			CASE(VK_NUMPAD3, KeyCode::NumPad3);
			CASE(VK_NUMPAD4, KeyCode::NumPad4);
			CASE(VK_NUMPAD5, KeyCode::NumPad5);
			CASE(VK_NUMPAD6, KeyCode::NumPad6);
			CASE(VK_NUMPAD7, KeyCode::NumPad7);
			CASE(VK_NUMPAD8, KeyCode::NumPad8);
			CASE(VK_NUMPAD9, KeyCode::NumPad9);
			CASE(VK_MULTIPLY, KeyCode::Multiply);
			CASE(VK_ADD, KeyCode::Add);
			CASE(VK_SUBTRACT, KeyCode::Subtract);
			CASE(VK_DECIMAL, KeyCode::Decimal);
			CASE(VK_DIVIDE, KeyCode::Divide);
			CASE(VK_F1, KeyCode::F1);
			CASE(VK_F2, KeyCode::F2);
			CASE(VK_F3, KeyCode::F3);
			CASE(VK_F4, KeyCode::F4);
			CASE(VK_F5, KeyCode::F5);
			CASE(VK_F6, KeyCode::F6);
			CASE(VK_F7, KeyCode::F7);
			CASE(VK_F8, KeyCode::F8);
			CASE(VK_F9, KeyCode::F9);
			CASE(VK_F10, KeyCode::F10);
			CASE(VK_F11, KeyCode::F11);
			CASE(VK_F12, KeyCode::F12);

			CASE(VK_F13, KeyCode::F13);
			CASE(VK_F14, KeyCode::F14);
			CASE(VK_F15, KeyCode::F15);
			CASE(VK_F16, KeyCode::F16);
			CASE(VK_F17, KeyCode::F17);
			CASE(VK_F18, KeyCode::F18);
			CASE(VK_F19, KeyCode::F19);
			CASE(VK_F20, KeyCode::F20);
			CASE(VK_F21, KeyCode::F21);
			CASE(VK_F22, KeyCode::F22);
			CASE(VK_F23, KeyCode::F23);
			CASE(VK_F24, KeyCode::F24);

			CASE(VK_NUMLOCK, KeyCode::NumLock);
			CASE(VK_SCROLL, KeyCode::ScrollLock);
		}
        return KeyCode::Null;
    }
#else
    KeyCode WinKeyToInputSystem(size_t key) { return KeyCode::Null; }
#endif
}