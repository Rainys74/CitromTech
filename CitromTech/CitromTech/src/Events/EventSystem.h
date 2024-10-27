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

        virtual CTL::String ToString() const {return GetEventTypeName();} // returns a nicely formatted string with values (by default returns the name)
    private:
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

    private:
        
    };
}