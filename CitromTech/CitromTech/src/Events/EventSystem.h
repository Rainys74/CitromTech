#pragma once

#include "Core.h"
#include "CTL/String.h"

namespace Citrom
{
    #define EVENT_CLASS_TYPE(CATEGORY, TYPE) virtual CATEGORY GetEventType() const override {return CATEGORY::TYPE;}           \
                                             virtual const char* GetEventCategoryName() const override {return #CATEGORY;}             \
                                             virtual const char* GetEventTypeName() const override {return #TYPE;}                     

    template<typename T> // T = Event Category
    class /*CITROM_API*/ Event
    {
    public:
        // EVENT_CLASS_TYPE
        virtual T GetEventType() const = 0;
        virtual const char* GetEventCategoryName() const = 0;
        virtual const char* GetEventTypeName() const = 0;

        virtual CTL::String ToString() const {return CTL::String(GetEventTypeName());} // returns a nicely formatted string with values (by default returns the name)
    protected:
        T m_Type;
        bool m_Handled = false;
    };

    enum class KeyEvents
    {
        KeyDown,
        KeyUp,
        KeyRepeat
    };

    class KeyDownEvent : public Event<KeyEvents>
    {
    public:
        EVENT_CLASS_TYPE(KeyEvents, KeyDown);
        
        KeyDownEvent(int keyCode) : m_KeyCode(keyCode) {}

        CTL::String ToString() const override 
        {
            CTL::String string("KeyDownEvent: ");
            string.Append(std::to_string(m_KeyCode).c_str());
            return string;
        }
    private:
        int m_KeyCode;
    };

    template<typename T>
    class EventListener
    {
    public:
        void (*OnEvent)(const Event<T>& event);
    };

    template<typename T>
    class EventDispatcher
    {
    public:
        void Dispatch(Event<T>& event)
        {
            CT_CORE_ASSERT_WARN(m_EventListener && m_EventListener->OnEvent, "Dispatching events to a null event listener or null event listener callback!");
            if (m_EventListener && m_EventListener->OnEvent)
                m_EventListener->OnEvent(event);
        }

        void AddListener(EventListener<T>* eventListener)
        {
            CT_CORE_ASSERT_WARN(eventListener, "Assigning null event listener!");
            m_EventListener = eventListener;
        }
    private:
        EventListener<T>* m_EventListener = nullptr;
    };

    /* TODO: implement based on this example: 
    class EventBus {
    public:
        using EventCallbackFn = std::function<void(const Event&)>;

        template<typename EventType>
        void AddListener(EventCallbackFn listener) {
            auto& listeners = m_Listeners[typeid(EventType).name()];
            listeners.push_back(listener);
        }

        template<typename EventType>
        void Dispatch(const EventType& event) {
            auto& listeners = m_Listeners[typeid(EventType).name()];
            for (auto& listener : listeners) {
                listener(event);
            }
        }

    private:
        std::unordered_map<const char*, std::vector<EventCallbackFn>> m_Listeners;
    };
    */
    class EventBus
    {
    public:

    };
}