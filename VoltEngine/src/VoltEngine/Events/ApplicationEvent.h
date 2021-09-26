#pragma once

#include "Event.h"

namespace Volt
{
    class CWindowResizeEvent : public CEvent
    {
    public:
        CWindowResizeEvent(uint32_t width, uint32_t height)
            : m_width(width), m_height(height) {}

        EVENT_CLASS_TYPE(WindowResize);
        EVENT_CLASS_CATEGORY(EventCategory_Application);

        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }

        virtual std::string ToString() const override
        {
            char buffer[32];
            sprintf_s(buffer, "WindowResizeEvent: [%d, %d]", m_width, m_height);

            std::string info(buffer);
            return info;
        }

    private:
        uint32_t m_width;
        uint32_t m_height;
    };

    class CWindowCloseEvent : public CEvent
    {
    public:
        EVENT_CLASS_TYPE(WindowClose);
        EVENT_CLASS_CATEGORY(EventCategory_Application);
    };
}
