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

    class MouseEvent : public Event<MouseEvents>
    {
    public:
        MouseEvent(EventMouseButton mouseButton) : m_MouseButton(mouseButton) {}

        CTL::String ToString() const override
        {
            CTL::String string(GetEventTypeName());
            string.Append("Event: ");
            string.Append(std::to_string((int)m_MouseButton).c_str());
            return string;
        }
    protected:
        EventMouseButton m_MouseButton;
    };

    class MouseDownEvent : public MouseEvent
    {
    public:
        EVENT_CLASS_TYPE(MouseEvents, MouseDown);
        //MouseDownEvent(EventMouseButton mouseButton) : MouseEvent(mouseButton) {}
        using MouseEvent::MouseEvent; // Inherit constructor
    };

    class MouseUpEvent : public MouseEvent
    {
    public:
        EVENT_CLASS_TYPE(MouseEvents, MouseUp);
        using MouseEvent::MouseEvent;
    };
}