#pragma once

#include "EventSystem.h"

#include <string>

namespace Citrom
{
    enum class WindowEvents
    {
        WindowClose,
        WindowResize,
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
}