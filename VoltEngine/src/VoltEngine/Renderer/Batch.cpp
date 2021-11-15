#include "Batch.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBatch.h"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    SharedPtr<IBatch> IBatch::Create(uint32_t maxNumTriangles)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLBatch>(maxNumTriangles);
        }
        VOLT_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }
}
