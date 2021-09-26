#pragma once
#include <sstream>
#include <cstdint>
#include <functional>
#include <string>

#include "VoltEngine/Core/Core.h"


namespace Volt
{
    enum class EEventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyTyped,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EEventCategory
    {
        None = 0,
        EventCategory_Application = BIT(0),
        EventCategory_Input = BIT(1),
        EventCategory_Keyboard = BIT(2),
        EventCategory_Mouse = BIT(3),
        EventCategory_MouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type) \
    static EEventType GetStaticType() {return EEventType::##type; } \
    virtual EEventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int32_t GetCategoryFlags() const { return category; }

    class CEvent
    {
        friend class CEventDispatcher;
    public:
        virtual EEventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int32_t GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EEventCategory category) const { return category & GetCategoryFlags(); }

        bool Handled = false;
    };

    class CEventDispatcher
    {
    public:
        CEventDispatcher(CEvent& e)
            : m_event(e) {}

        template <typename T>
        bool Dispatch(std::function<bool(T&)> function)
        {
            bool success = false;
            if (m_event.GetEventType() == T::GetStaticType())
            {
                // ReSharper disable once CppCStyleCast
                m_event.Handled = function(*(T*)&m_event);
                success = true;
            }
            return success;
        }

    private:
        CEvent& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const CEvent& e) { return os << e.ToString(); }
}
