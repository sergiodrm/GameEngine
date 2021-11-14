#pragma once

#include <functional>

#include "Core.h"

namespace Volt
{
    struct SWindowData
    {
        using WindowEvent = std::function<void(class CEvent& e)>;

        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowEvent EventFunction;

        SWindowData(const std::string& title = "Volt Engine", uint32_t width = 1200, uint32_t height = 720)
            : Title(title), Width(width), Height(height) {}
    };

    class IWindow
    {
    public:
        static SharedPtr<IWindow> Create(const SWindowData& windowProps = SWindowData());

        virtual ~IWindow() = default;

        virtual void OnUpdate() = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetEventFunction(const SWindowData::WindowEvent& eventFunction) = 0;

        virtual void* GetNativeWindow() const = 0;
    };
}
