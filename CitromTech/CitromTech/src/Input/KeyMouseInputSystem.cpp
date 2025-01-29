#include "KeyMouseInputSystem.h"

namespace Citrom::Input
{
    float g_LastMousePositionX, g_LastMousePositionY;
    float g_CurrentMousePositionX, g_CurrentMousePositionY;

    bool g_KeysDown[(size_t)KeyCode::Count];
    bool g_MButtonsDown[(size_t)MouseButton::Count];

    //bool s_KeyStateJustChanged

    bool SimpleInput::GetKey(KeyCode keyCode)
    {
        return g_KeysDown[(size_t)keyCode];
    }
    bool SimpleInput::GetMouseButton(MouseButton mouseButton)
    {
        return g_MButtonsDown[(size_t)mouseButton];
    }

    float SimpleInput::GetMouseDeltaX()
    {
        const float deltaX = g_CurrentMousePositionX - g_LastMousePositionX;
        return deltaX;
    }
    float SimpleInput::GetMouseDeltaY()
    {
        const float deltaY = g_CurrentMousePositionY - g_LastMousePositionY;
        return deltaY;
    }

    SimpleInputManager::SimpleInputManager()
    {
        m_MouseEventsListener.OnEvent = [](const Event<MouseEvents>& event)
        {
            if (event.GetEventType() == MouseEvents::MouseDown)
            {
                const MouseDownEvent& transformedEvent = (const MouseDownEvent&)event;
                g_MButtonsDown[(size_t)transformedEvent.mouseButton] = true;
            }
            else if (event.GetEventType() == MouseEvents::MouseUp)
            {
                const MouseUpEvent& transformedEvent = (const MouseUpEvent&)event;
                g_MButtonsDown[(size_t)transformedEvent.mouseButton] = false;
            }
            else if (event.GetEventType() == MouseEvents::MouseMove)
            {
                const MouseMoveEvent& transformedEvent = (const MouseMoveEvent&)event;
                g_CurrentMousePositionX = transformedEvent.x;
                g_CurrentMousePositionY = transformedEvent.y;
            }
        };
        EventBus::GetDispatcher<MouseEvents>()->AddListener(&m_MouseEventsListener);

        m_KeyEventsListener.OnEvent = [](const Event<KeyEvents>& event)
        {
            if (event.GetEventType() == KeyEvents::KeyDown)
            {
                const KeyDownEvent& transformedEvent = (const KeyDownEvent&)event;
                g_KeysDown[(size_t)transformedEvent.keyCode] = true;
            }
            else if (event.GetEventType() == KeyEvents::KeyUp)
            {
                const KeyUpEvent& transformedEvent = (const KeyUpEvent&)event;
                g_KeysDown[(size_t)transformedEvent.keyCode] = false;
            }
        };
        EventBus::GetDispatcher<KeyEvents>()->AddListener(&m_KeyEventsListener);
    }
    SimpleInputManager::~SimpleInputManager()
    {
        EventBus::GetDispatcher<MouseEvents>()->RemoveListener(&m_MouseEventsListener);
        EventBus::GetDispatcher<KeyEvents>()->RemoveListener(&m_KeyEventsListener);
    }
    void SimpleInputManager::Update()
    {
        g_LastMousePositionX = g_CurrentMousePositionX;
        g_LastMousePositionY = g_CurrentMousePositionY;
    }
}