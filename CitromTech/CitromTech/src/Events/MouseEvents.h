#pragma once

#include "EventSystem.h"

#include "Input/MouseInput.h"

#include <string>

namespace Citrom
{
    enum class MouseEvents
    {
        MouseDown,
        MouseUp
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
}