#pragma once
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class IGraphicsContext
    {
    public:
        static Ref<IGraphicsContext> Create();

        virtual void Init(void* windowContext) = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetWindowContext(void* windowContext) = 0;
    };
}
