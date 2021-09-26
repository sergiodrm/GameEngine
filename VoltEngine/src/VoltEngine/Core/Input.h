#pragma once
#include <cstdint>

namespace Volt
{
    class IInput
    {
    public:
        static bool IsKeyPressed(int32_t key);
        static bool IsMouseButtonPressed(int32_t button);
        static float GetMouseX();
        static float GetMouseY();
        static void GetMousePosition(float& x, float& y);
    };
}
