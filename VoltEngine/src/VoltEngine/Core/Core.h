#pragma once

#if _WIN32
#ifndef _WIN64
#error "x86 builds are not supported."
#endif
#endif
#include <memory>


namespace Volt
{
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
