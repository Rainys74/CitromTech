#pragma once

#include "EventSystem.h"

#include "Input/MouseInput.h"

#include <string>

namespace Citrom
{
    enum class MouseEvents
    {
        MouseDown,
        MouseUp,
        MouseMove
    };
    using EventMouseButton = Input::MouseButton;

    class MouseEventBase : public Event<MouseEvents>
    {
    public:
        CTL::String ToString() const override
        {
            CTL::String string(GetEventTypeName());
            string.Append("Event: ");
            string.Append(std::to_string((int)mouseButton).c_str());
            return string;
        }
    public:
        EventMouseButton mouseButton;
    };

    class MouseDownEvent : public MouseEventBase
    {
    public:
        EVENT_CLASS_TYPE(MouseEvents, MouseDown);
    };

    class MouseUpEvent : public MouseEventBase
    {
    public:
        EVENT_CLASS_TYPE(MouseEvents, MouseUp);
    };

    class MouseMoveEvent : public Event<MouseEvents>
    {
    public:
        EVENT_CLASS_TYPE(MouseEvents, MouseMove);

        CTL::String ToString() const override
        {
            CTL::String string(GetEventTypeName());
            string.Append("Event: ");
            string.Append(std::to_string(x).c_str());
            string.Append(", ");
            string.Append(std::to_string(y).c_str());
            return string;
        }
    public:
        int x, y;
    };
}