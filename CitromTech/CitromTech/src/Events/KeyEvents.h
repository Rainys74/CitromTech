#pragma once

#include "EventSystem.h"
#include "Input/KeyboardInput.h"

#include <string>

namespace Citrom
{
    enum class KeyEvents
    {
        KeyDown,
        KeyUp,
        KeyRepeat
    };

    using EventKeyCode = Input::KeyCode; //uint32;
    class KeyEventBase : public Event<KeyEvents>
    {
    public:
        CTL::String ToString() const override
        {
            CTL::String string(GetEventTypeName());
            string.Append("Event: ");
            string.Append(std::to_string((uint32)keyCode).c_str());
            return string;
        }
    public:
        EventKeyCode keyCode;
    };

    class KeyDownEvent : public KeyEventBase
    {
    public:
        EVENT_CLASS_TYPE(KeyEvents, KeyDown);
    };

    class KeyUpEvent : public KeyEventBase
    {
    public:
        EVENT_CLASS_TYPE(KeyEvents, KeyUp);
    };

    class KeyRepeatEvent : public KeyEventBase
    {
    public:
        EVENT_CLASS_TYPE(KeyEvents, KeyRepeat);
    };
}