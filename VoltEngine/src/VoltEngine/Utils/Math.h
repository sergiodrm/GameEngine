#pragma once

namespace Volt
{
    class CMath
    {
    public:
        template <typename T>
        static T Clamp(const T& value, const T& min, const T& max)
        {
            return value <= min ? min : value >= max ? max : value;
        }

        template <typename T>
        static bool IsNearlyEqual(T first, T second, float e = 1e-6f)
        {
            return first >= second - e && first <= second + e;
        }

        template <typename T>
        static bool IsNearlyZero(T value, float e = 1e-6f)
        {
            return IsNearlyEqual(value, 0.f, e);
        }
    };
}
