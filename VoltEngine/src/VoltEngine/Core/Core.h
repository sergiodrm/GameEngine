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
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Unique = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Unique<T> CreateUnique(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}
