#pragma once

namespace Volt
{
    template <typename T, typename C>
    static T* Cast(C* object)
    {
        return object != nullptr && (C::GetStaticType().template IsSubTypeOf<T>() || T::GetStaticType().template IsSubTypeOf<C>())
                   ? static_cast<T*>(object)
                   : nullptr;
    }

    template <typename T, typename C>
    static const T* Cast(const C* object)
    {
        return object != nullptr && (C::GetStaticType().template IsSubTypeOf<T>() || T::GetStaticType().template IsSubTypeOf<C>())
                   ? static_cast<const T*>(object)
                   : nullptr;
    }
}
