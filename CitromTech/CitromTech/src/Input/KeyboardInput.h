#pragma once

#include "Core.h"

#include "CTL/String.h"

namespace Citrom::Input
{
	enum class KeyCode
	{
        Null,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Backspace,
        Tab,
        Return, // Enter

        Shift,
        LShift,
        RShift,

        Ctrl,
        LCtrl,
        RCtrl,

        Alt,
        LAlt,
        RAlt,

        Pause,
        CapsLock,
        Escape,
        Space,
        PageUp,
        PageDown,
        End,
        Home,

        Up,
        Left,
        Down,
        Right,

        PrintScreen,
        Insert,
        Delete,

        NumPad0,
        NumPad1,
        NumPad2,
        NumPad3,
        NumPad4,
        NumPad5,
        NumPad6,
        NumPad7,
        NumPad8,
        NumPad9,
        Multiply, // numpad *
        Add,      // numpad +
        Subtract, // numpad -
        Decimal,  // numpad .
        Divide,   // numpad /
        NumLock,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,      // should go up to 24

        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,

        ScrollLock,

        Alpha1,
        Alpha2,
        Alpha3,
        Alpha4,
        Alpha5,
        Alpha6,
        Alpha7,
        Alpha8,
        Alpha9,
        Alpha0,

        BackQuote,   // backtick, tilde or that console key `
        AlphaMinus,  // alpha -
        AlphaEqual,  // alpha =
        Slash,
        BackSlash,
        Apps,        // Applications key, Menu key, key that looks like a cupboard

        LeftBracket,
        RightBracket,
        SemiColon,
        Apostrophe,  // quote '
        Comma,
        Period,
        Start,       // Windows - Windows, Super - Linux, Command - MacOS

        Count
	};

    KeyCode WinKeyToInputSystem(size_t key);
    KeyCode GLFWKeyToInputSystem(int key);

    std::string KeyCodeToString(KeyCode keyCode);
}