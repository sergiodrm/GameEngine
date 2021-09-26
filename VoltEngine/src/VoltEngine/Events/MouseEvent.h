#pragma once

#include "Event.h"

namespace Volt
{
    class CMouseMovedEvent : public CEvent
    {
    public:
        CMouseMovedEvent(float x, float y)
            : m_x(x), m_y(y) {}

        EVENT_CLASS_CATEGORY(EventCategory_Mouse);
        EVENT_CLASS_TYPE(MouseMoved);

        float GetX() const { return m_x; }
        float GetY() const { return m_y; }

        virtual std::string ToString() const override
        {
            std::stringstream Info;
            Info << "Mouse moved event: " << m_x << ", " << m_y;
            return Info.str();
        }

    private:
        float m_x;
        float m_y;
    };

    class CMouseScrolledEvent : public CEvent
    {
    public:
        CMouseScrolledEvent(float x, float y)
            : m_deltaX(x), m_deltaY(y) {}

        EVENT_CLASS_CATEGORY(EventCategory_Mouse);
        EVENT_CLASS_TYPE(MouseScrolled);

        float GetDeltaX() const { return m_deltaX; }
        float GetDeltaY() const { return m_deltaY; }

        virtual std::string ToString() const override
        {
            std::stringstream Info;
            Info << "Mouse scrolled event: " << m_deltaX << ", " << m_deltaY;
            return Info.str();
        }

    private:
        float m_deltaX;
        float m_deltaY;
    };

    class CMouseButtonEvent : public CEvent
    {
    public:
        CMouseButtonEvent(int32_t button)
            : m_button(button) {}

        EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_MouseButton | EventCategory_Input);

        int32_t GetButton() const { return m_button; }
    private:
        int32_t m_button;
    };

    class CMouseButtonPressedEvent : public CMouseButtonEvent
    {
    public:
        EVENT_CLASS_TYPE(MouseButtonPressed);

        virtual std::string ToString() const override
        {
            std::stringstream Info;
            Info << "Mouse button pressed: " << GetButton();
            return Info.str();
        }
    };

    class CMouseButtonReleasedEvent : public CMouseButtonEvent
    {
    public:
        EVENT_CLASS_TYPE(MouseButtonReleased);

        virtual std::string ToString() const override
        {
            std::stringstream Info;
            Info << "Mouse button released: " << GetButton();
            return Info.str();
        }
    };
}
