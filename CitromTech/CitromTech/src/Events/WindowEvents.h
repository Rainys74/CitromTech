#pragma once

#include "EventSystem.h"

#include <string>

namespace Citrom
{
    enum class WindowEvents
    {
        WindowClose,
        WindowResize,
        WindowMove,
        WindowFocus
    };

    class WindowCloseEvent : public Event<WindowEvents>
    {
    public:
        EVENT_CLASS_TYPE(WindowEvents, WindowClose);

        CTL::String ToString() const override
        {
            CTL::String string("WindowCloseEvent: ");
            string.Append(std::to_string(exitCode).c_str());
            return string;
        }
    public:
        int exitCode = 0;
    };

    class WindowResizeEvent : public Event<WindowEvents>
    {
    public:
        EVENT_CLASS_TYPE(WindowEvents, WindowResize);

        CTL::String ToString() const override
        {
            CTL::String string("WindowResizeEvent: ");
            string.Append(std::to_string(width).c_str());
            string.Append(", ");
            string.Append(std::to_string(height).c_str());
            return string;
        }
    public:
        uint32 width = 0;
        uint32 height = 0;
    };

    class WindowMoveEvent : public Event<WindowEvents>
    {
    public:
        EVENT_CLASS_TYPE(WindowEvents, WindowMove);

        CTL::String ToString() const override
        {
            CTL::String string("WindowMoveEvent: ");
            string.Append(std::to_string(x).c_str());
            string.Append(", ");
            string.Append(std::to_string(y).c_str());
            return string;
        }
    public:
        int32 x = 0;
        int32 y = 0;
    };

    class WindowFocusEvent : public Event<WindowEvents>
    {
    public:
        EVENT_CLASS_TYPE(WindowEvents, WindowFocus);

        CTL::String ToString() const override
        {
            CTL::String string("WindowFocusEvent: ");
            string.Append(std::to_string(state).c_str());
            return string;
        }
    public:
        bool state = true; // whether focus was lost or gained
    };
}