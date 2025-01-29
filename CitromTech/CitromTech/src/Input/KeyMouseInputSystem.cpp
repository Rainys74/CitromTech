#include "KeyMouseInputSystem.h"

namespace Citrom::Input
{
    float SimpleInput::s_LastMousePositionX;
    float SimpleInput::s_LastMousePositionY;
    float SimpleInput::s_CurrentMousePositionX;
    float SimpleInput::s_CurrentMousePositionY;

    bool SimpleInput::s_KeysDown[(size_t)KeyCode::Count];
    bool SimpleInput::s_MButtonsDown[(size_t)MouseButton::Count];
    // or use states like CitromTech2D which requires updating, and calling get functions between the registering and update?

    EventListener<KeyEvents> SimpleInput::s_KeyEventsListener;
    EventListener<MouseEvents> SimpleInput::s_MouseEventsListener;

    bool SimpleInput::s_Initialized = false;

    void SimpleInput::TryInitialize()
    {
        if (!s_Initialized)
        {
            s_MouseEventsListener.OnEvent = [](const Event<MouseEvents>& event) 
            {
                if (event.GetEventType() == MouseEvents::MouseDown)
                {
                    const MouseDownEvent& transformedEvent = (const MouseDownEvent&)event;
                    s_MButtonsDown[(size_t)transformedEvent.mouseButton] = true;
                }
                else if (event.GetEventType() == MouseEvents::MouseUp)
                {
                    const MouseUpEvent& transformedEvent = (const MouseUpEvent&)event;
                    s_MButtonsDown[(size_t)transformedEvent.mouseButton] = false;
                }
                else if (event.GetEventType() == MouseEvents::MouseMove)
                {
                    const MouseMoveEvent& transformedEvent = (const MouseMoveEvent&)event;
                    s_CurrentMousePositionX = transformedEvent.x;
                    s_CurrentMousePositionY = transformedEvent.y;
                }
            };
            EventBus::GetDispatcher<MouseEvents>()->AddListener(&s_MouseEventsListener);

            s_Initialized = true;
        }
    }

    bool SimpleInput::GetKey(KeyCode keyCode)
    {
        return false;
    }
    bool SimpleInput::GetMouseButton(MouseButton mouseButton)
    {
        TryInitialize();
        return s_MButtonsDown[(size_t)mouseButton];
    }

    float SimpleInput::GetMouseDeltaX()
    {
        const float deltaX = s_CurrentMousePositionX - s_LastMousePositionX;
        s_LastMousePositionX = s_CurrentMousePositionX;
        return deltaX;
    }
    float SimpleInput::GetMouseDeltaY()
    {
        const float deltaY = s_CurrentMousePositionY - s_LastMousePositionY;
        s_LastMousePositionY = s_CurrentMousePositionY;
        return deltaY;
    }
}