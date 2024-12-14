#pragma once

#include "Core.h"

namespace Citrom::Input
{
    enum class MouseButton
    {
        Null,

        LeftButton,
        RightButton,
        MiddleButton,

        // Reorganized on top because Mouse4 = 4, Mouse5 = 5.
        Mouse4, // XButton1 (back button)
        Mouse5, // XButton2 (forward button)

        // Additional buttons
        Mouse6,
        Mouse7,
        Mouse8,
        Mouse9,
        Mouse10,
        Mouse11,
        Mouse12,

        Mouse13,
        Mouse14,
        Mouse15,
        Mouse16,
        Mouse17,
        Mouse18,
        Mouse19,
        Mouse20,

        // Scrolling
        WheelUp,
        WheelDown,

        // Horizontal Scrolling
        WheelLeft,
        WheelRight,

        Count
    };
}