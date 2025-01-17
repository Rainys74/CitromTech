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

    using EventKeyCode = uint32; //Input::KeyCode;
    class KeyDownEvent : public Event<KeyEvents>
    {
    public:
        EVENT_CLASS_TYPE(KeyEvents, KeyDown);
        
        KeyDownEvent(EventKeyCode keyCode) : m_KeyCode(keyCode) {}

        CTL::String ToString() const override 
        {
            CTL::String string("KeyDownEvent: ");
            string.Append(std::to_string((uint32)m_KeyCode).c_str());
            return string;
        }
    private:
        EventKeyCode m_KeyCode;
    };
}