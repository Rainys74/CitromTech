#pragma once

#include "Core.h"
#include "CTL/String.h"
#include "CTL/DArray.h"
#include "CTL/HashMap.h"

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

        // This function should only really be called for debugging
        virtual CTL::String ToString() const {return CTL::String(GetEventTypeName());} // returns a nicely formatted string with values (by default returns the name)
    protected:
        bool m_Handled = false;
    };

    template<typename T>
    class EventListener
    {
    public:
        void (*OnEvent)(const Event<T>& event);
        
        void operator()(const Event<T>& event) const
        {
            OnEvent(event);
        }
    };

    template<typename T>
    class EventDispatcher
    {
    public:
        // Register/Send events
        void Dispatch(Event<T>& event)
        {
            for (EventListener<T>* eventListener : m_EventListeners)
            {
                CT_CORE_ASSERT_WARN(eventListener && eventListener->OnEvent, "Dispatching events to a null event listener or null event listener callback!");
                if (eventListener && eventListener->OnEvent)
                {
                    eventListener->OnEvent(event);
                }
            }
        }

        // Subscribe
        void AddListener(EventListener<T>* eventListener)
        {
            CT_CORE_ASSERT_WARN(eventListener, "Adding null event listener!");
            m_EventListeners.PushBack(eventListener);
        }
    private:
        CTL::DArray<EventListener<T>*> m_EventListeners;
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
    // TODO: improve this someday
    class EventBus
    {
    public:
        using EventListenerFunctionPtr = void(*)(const Event<void>&);

        static EventBus* GetInstance()
        {
            static EventBus instance;
            return &instance;
        }

        template<typename T>
        void AddListener(EventListener<T>* eventListener)
        {
            m_Listeners[typeid(T).name()].PushBack((EventListenerFunctionPtr)eventListener->OnEvent);
        }

        template<typename T>
        void Dispatch(Event<T>& event)
        {
            for (EventListenerFunctionPtr eventListenerCallback : m_Listeners[typeid(T).name()])
            {
                if (eventListenerCallback)
                    eventListenerCallback((const Event<void>&)event);
            }
        }

    private:
        CTL::HashMap<const char*, CTL::DArray<EventListenerFunctionPtr>> m_Listeners;
    };
}