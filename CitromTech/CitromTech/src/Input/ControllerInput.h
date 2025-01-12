#pragma once

#include "Core.h"

namespace Citrom::Input
{
    // Gamepads
    enum class ControllerAxes
    {
        Null,

        LeftStickX,
        LeftStickY,

        RightStickX,
        RightStickY,

        LeftTrigger, // L2
        RightTrigger, // R2

        Count
    };

    enum class ControllerButtons
    {
        Null,

        // Xbox = Playstation = Labels
        Top, // Y = Triangle = Alt-Action
        Left, // X = Square = Special
        Bottom, // A = X/Cross = Action
        Right, // B = Circle/O = Back

        LeftShoulder, // LB = L1
        RightShoulder, // RB = R1

        DPadUp,
        DPadLeft,
        DPadBottom,
        DPadRight,

        LeftStickButton, // L3
        RightStickButton, // R3

        Options, // Menu
        //Share, // View

        Count
    };
}