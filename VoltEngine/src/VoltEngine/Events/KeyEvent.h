#pragma once
#include "Event.h"

namespace Volt
{
    class CKeyEvent : public CEvent
    {
    public:
        CKeyEvent(int32_t key)
            : m_key(key) {}

        EVENT_CLASS_CATEGORY(EventCategory_Input | EventCategory_Keyboard);

        int32_t GetKey() const { return m_key; }

    private:
        int32_t m_key;
    };

    class CKeyPressedEvent : public CKeyEvent
    {
    public:
        CKeyPressedEvent(int32_t key)
            : CKeyEvent(key) {}

        EVENT_CLASS_TYPE(KeyPressed);

        virtual std::string ToString() const override
        {
            std::stringstream Info;
            Info << "Key pressed: " << GetKey();
            return Info.str();
        }
    };

    class CKeyReleasedEvent : public CKeyEvent
    {
    public:
        CKeyReleasedEvent(int32_t key)
            : CKeyEvent(key) {}

        EVENT_CLASS_TYPE(KeyReleased);

        virtual std::string ToString() const override
        {
            std::stringstream Info;
            Info << "Key released: " << GetKey();
            return Info.str();
        }
    };
}
