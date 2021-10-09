#include "OpenGLRendererAPI.h"

#include "glad/glad.h"
#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/VertexArray.h"

namespace Volt
{
    static void OpenGLMessageCallback(GLenum source, GLenum type, GLenum id, GLenum severity,
                                      GLsizei length, const GLchar* message, const void* userParam)
    {
        VOLT_LOG(Error, "GL_CALLBACK: {0} msg : {1}", type == GL_DEBUG_TYPE_ERROR ? "** GL_ERROR **" : "", message);
    }

    void COpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);
    }

    void COpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void COpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void COpenGLRendererAPI::DrawIndexed(const Ref<IVertexArray>& vertexArray, uint32_t indexCount)
    {
        const uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void COpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }
}
