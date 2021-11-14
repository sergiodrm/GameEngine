#pragma once

#if _WIN32
#ifndef _WIN64
#error "x86 builds are not supported."
#endif
#endif

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <cassert>

#define EXPAND(x) (x)
#define BIT(x)  1 << (x)
#define BIND_FUNCTION(fn) [this](auto& ... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


namespace Volt
{
    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr SharedPtr<T> CreateSharedPtr(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using UniquePtr = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr UniquePtr<T> CreateUnique(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    class WeakPtr : public std::weak_ptr<T>
    {
    public:
        T* operator->() { return this->lock() ? this->lock().get() : nullptr; }
        const T* operator->() const { return this->lock() ? this->lock().get() : nullptr; }
    };
}
