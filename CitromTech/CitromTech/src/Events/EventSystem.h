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

        //virtual CTL::String ToString() const {return GetEventTypeName();} // returns a nicely formatted string with values (by default returns the name)
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
}