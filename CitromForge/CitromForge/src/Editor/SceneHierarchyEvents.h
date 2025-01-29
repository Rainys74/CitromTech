#pragma once

#include "Events/EventSystem.h"

#include <string>

enum class SceneHierarchyEvents
{
    EntitySelect
};

using EventEntityHandleType = uint32;
class EntitySelectEvent : public Citrom::Event<SceneHierarchyEvents>
{
public:
    EVENT_CLASS_TYPE(SceneHierarchyEvents, EntitySelect);

    CTL::String ToString() const override
    {
        CTL::String string("EntitySelectEvent: ");
        string.Append(std::to_string(entityHandleID).c_str());
        return string;
    }
public:
    EventEntityHandleType entityHandleID = 0;
};